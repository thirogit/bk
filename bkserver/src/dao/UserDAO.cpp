#include "stdafx.h"
#include "UserDAO.h"
#include <progress/SafeSimpleProgress.h>
#include <progress/SafeComplexProgress.h>

#include "../db/SQLException.h"
#include "../db/query/SQLQuery.h"
#include <string/TextUtils.h>
#include <boost/scope_exit.hpp>
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>
#include <data/IIdable.h>
#include <configuration/passportrecognition/PassportFields.h>
#include <configuration/passportrecognition/PassportFieldDescriptor.h>

UserDAO::OrderSpecifierWithPosition::OrderSpecifierWithPosition(const std::wstring& sFieldCode, int position) : OrderSpecifier(sFieldCode)
{
	m_position = position;
}

int UserDAO::OrderSpecifierWithPosition::position() const
{
	return m_position;
}

//#################################################################################################

bool UserDAO::LoadPrintProfiles(NewPtrFlushArray<PrintProfile>& result, uint32_t userId, ComplexProgress* pProgress /*= NULL*/)
{
	PrintProfile *oneRow = NULL;		
	int colPROFILENAME,colDOCUMENTCODE,colPROFILEID;
	PrintOption* option = NULL;
	
	SafeComplexProgress progress(pProgress);
	progress.SetTaskCount(3);
	
	Transaction transaction = OpenTransaction();
	OptionsMap optionsMap;
	OrderSpecifiersMap orderSpecMap;
	BOOST_SCOPE_EXIT(&optionsMap, &orderSpecMap)
	{
		DeepDelete(optionsMap);
		DeepDelete(orderSpecMap);
	}
	BOOST_SCOPE_EXIT_END


	SafeSimpleProgress optionsProgress(progress.BeginTask(L"loading_options"));
	if (!LoadOptions(transaction, optionsMap, userId, &optionsProgress))
	{
		return false;
	}

	SafeSimpleProgress orderSpecProgress(progress.BeginTask(L"loading_order_specifiers"));
	if (!LoadOrderSpecifiers(transaction, orderSpecMap, userId, &orderSpecProgress))
	{
		return false;
	}	

	progress.EndTask();
	SafeSimpleProgress profilesProgress(progress.BeginTask(L"loading_profiles"));

	SQLQuery q;
	q << L"SELECT COUNT(*) FROM user_print_profiles WHERE user_owner = %0";
	q.parse();
		
	int step = 0;
	int steps = transaction->QueryScalar(q.str(userId), 0);
	if(steps < 0) return false;
		
	profilesProgress.SetSteps(steps);

	result.RemoveAll();

	std::function<bool(OrderSpecifierWithPosition*, OrderSpecifierWithPosition*)> comparator = [](OrderSpecifierWithPosition* o1, OrderSpecifierWithPosition* o2) -> bool
	{
		return o1->position() > o2->position();
	};

	try
	{		
		q.reset();
		q << L"SELECT * FROM user_print_profiles WHERE user_owner = %0";
		q.parse();

		Cursor cursor = transaction->Query(q.str(userId));

		colPROFILENAME = cursor.colIndex(L"profile_name");
		colDOCUMENTCODE = cursor.colIndex(L"document_code");
		colPROFILEID = cursor.colIndex(L"profile_id");
			
		while (cursor.nextRow())
		{	
			oneRow = new PrintProfile(cursor.getColInt(colPROFILEID));
				
			oneRow->SetProfileName(cursor.getColString(colPROFILENAME));
			oneRow->SetDocumentCode(cursor.getColString(colDOCUMENTCODE));
			
			auto optionsRange = optionsMap.equal_range(oneRow->GetProfileId());			
			auto optionIt = optionsRange.first;
			while (optionIt != optionsRange.second)
			{
				option = optionIt->second;
				oneRow->AddOption(option->name())->set(option->get());
				optionIt++;
			}

			std::vector<OrderSpecifierWithPosition*> unsortedOrderSpecifiers;
			auto orderSpecifiersRange = orderSpecMap.equal_range(oneRow->GetProfileId());
			auto orderSpecifierIt = orderSpecifiersRange.first;
			while (orderSpecifierIt != orderSpecifiersRange.second)
			{
				unsortedOrderSpecifiers.push_back(orderSpecifierIt->second);
				orderSpecifierIt++;
			}
			std::sort(unsortedOrderSpecifiers.begin(), unsortedOrderSpecifiers.end(), comparator);
			
			BOOST_FOREACH(OrderSpecifierWithPosition* specifier, unsortedOrderSpecifiers)
			{
				oneRow->AddOrderSpecifier(specifier->fieldCode())->asc(specifier->asc());
			}
			
			result.Add(oneRow);
			oneRow = NULL;
			if (!profilesProgress.Step()) return false;
		}

		progress.EndTask();
	}
	catch (SQLException& e)
	{
		if(oneRow) delete oneRow;
		
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}

bool UserDAO::LoadOptions(Transaction transaction, OptionsMap& result, uint32_t userId, SimpleProgress* pProgress)
{
	PrintOption *oneRow = NULL;

	int colOPTIONCODE, colOPTIONVALUE, colPROFILEID;
	uint32_t profileId;

	SQLQuery q;
	q << L"SELECT COUNT(*) FROM user_print_profile_options WHERE user_owner = %0";
	q.parse();

	int step = 0;
	int steps = transaction->QueryScalar(q.str(userId), 0);
	if (steps < 0) return false;

	SafeSimpleProgress progress(pProgress);
	progress.SetSteps(steps);

	try
	{
		q.reset();
		q << L"SELECT * FROM user_print_profile_options WHERE user_owner = %0";
		q.parse();

		Cursor cursor = transaction->Query(q.str(userId));

		colOPTIONCODE = cursor.colIndex(L"option_code");
		colOPTIONVALUE = cursor.colIndex(L"option_value");
		colPROFILEID = cursor.colIndex(L"profile");
		
		while (cursor.nextRow())
		{
			profileId = cursor.getColInt(colPROFILEID);
			oneRow = new PrintOption(cursor.getColString(colOPTIONCODE));
			oneRow->set(cursor.getColString(colOPTIONVALUE));

			result.insert(std::make_pair(profileId, oneRow));

			oneRow = NULL;
			if (!progress.Step()) return false;
		}
	}
	catch (SQLException& e)
	{
		if (oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;

}


bool UserDAO::DeleteOrderSpecifiers(Transaction transaction, uint32_t profileId)
{
	SQLQuery q;
	q << L"DELETE FROM user_print_profile_order WHERE profile = %0";
	q.parse();
	return transaction->Delete(q.str(profileId).c_str());
}

void UserDAO::InsertOrderSpecifier(Transaction transaction, uint32_t userId, uint32_t profileId, int position, const OrderSpecifier* specifier)
{
	SQLQuery q;
	q << L"INSERT INTO  user_print_profile_order ("
		L"field_code,"		//0
		L"profile,"			//1
		L"user_owner,"		//2
		L"\"collation\","		//3		
		L"position"			//4		
		L") VALUES ("
		L"%0q,"
		L"%1,"
		L"%2,"
		L"%3,"
		L"%4"
		L")";

	q.parse();

	SQLQueryParms params;

	params << specifier->fieldCode()    //0
		<< profileId					//1
		<< userId						//2
		<< (specifier->asc() ? 1 : -1)	//3
		<< position						//4
		;

	transaction->Insert(q.str(params).c_str());
}


bool UserDAO::LoadOrderSpecifiers(Transaction transaction, OrderSpecifiersMap& result, uint32_t userId, SimpleProgress* pProgress)
{
	OrderSpecifierWithPosition *oneRow = NULL;

	int colFIELDCODE, colCOLLATION, colPROFILEID,colPOSITION;
	uint32_t profileId;
	
	SQLQuery q;
	q << L"SELECT COUNT(*) FROM user_print_profile_order WHERE user_owner = %0";
	q.parse();

	int step = 0;
	int steps = transaction->QueryScalar(q.str(userId), 0);
	if (steps < 0) return false;

	SafeSimpleProgress progress(pProgress);
	progress.SetSteps(steps);

	try
	{
		q.reset();
		q << L"SELECT * FROM user_print_profile_order WHERE user_owner = %0";
		q.parse();

		Cursor cursor = transaction->Query(q.str(userId));

		colFIELDCODE = cursor.colIndex(L"field_code");
		colCOLLATION = cursor.colIndex(L"collation");
		colPROFILEID = cursor.colIndex(L"profile");
		colPOSITION = cursor.colIndex(L"position");

		while (cursor.nextRow())
		{
			profileId = cursor.getColInt(colPROFILEID);
			oneRow = new OrderSpecifierWithPosition(cursor.getColString(colFIELDCODE), cursor.getColInt(colPOSITION));
			oneRow->asc(cursor.getColInt(colCOLLATION) > 0);

			result.insert(std::make_pair(profileId, oneRow));

			oneRow = NULL;
			if (!progress.Step()) return false;
		}
	}
	catch (SQLException& e)
	{
		if (oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;

}


void UserDAO::UpdatePrintProfile(Transaction transaction, IPrintProfile* profile)
{
	SQLQuery userQ;
	userQ << L"SELECT user_owner FROM user_print_profiles WHERE profile_id = " << profile->GetProfileId();
	uint32_t userId = transaction->QueryScalar(userQ.str(), NULL_ID);
	uint32_t profileId = profile->GetProfileId();
	DeleteOptions(transaction, profileId);
	DeleteOrderSpecifiers(transaction, profileId);
	
	PtrEnumerator<PrintOption> enumOption = profile->EnumOptions();
	while (enumOption.hasNext())
	{
		InsertOption(transaction, userId, profileId, *enumOption);
		enumOption.advance();		
	}

	PtrEnumerator<OrderSpecifier> enumOrderSpecifiers = profile->EnumOrderSpecifiers();
	int position = 1;
	while (enumOrderSpecifiers.hasNext())
	{
		InsertOrderSpecifier(transaction, userId, profileId, position, *enumOrderSpecifiers);
		enumOrderSpecifiers.advance();
		position++;
	}


	SQLQuery stmt;
	stmt << L"UPDATE user_print_profiles SET " 
			L"profile_name = %0q"			//0
			L" WHERE profile_id = %1;";

	stmt.parse();

	SQLQueryParms params;

	params << profile->GetProfileName()			//0
		<< profileId							//1		
		;

	transaction->Update(stmt.str(params).c_str());


}

bool UserDAO::DeleteOptions(Transaction transaction, uint32_t profileId)
{
	SQLQuery q;
	q << L"DELETE FROM user_print_profile_options WHERE profile = %0";
	q.parse();
	return transaction->Delete(q.str(profileId).c_str());
}

PrintProfile* UserDAO::InsertPrintProfile(Transaction transaction, uint32_t userId, IPrintProfile* profile)
{
	uint32_t profileId = NextSeqValue(transaction, L"printprofiles_seq");

	std::auto_ptr<PrintProfile> newProfile(new PrintProfile(profileId));
	newProfile->CopyFrom(profile);
	
	SQLQuery q;
	q << L"INSERT INTO user_print_profiles ("
		 L"profile_id,"		//0
		 L"profile_name,"	//1
		 L"user_owner,"		//2
		 L"document_code"	//3		
		 L") VALUES ("
		 L"%0,"
		 L"%1q,"
		 L"%2,"
		 L"%3q"
		 L")";

	q.parse();

	SQLQueryParms params;

	params << profileId					//0
		<< newProfile->GetProfileName()	//1
		<< userId						//2
		<< newProfile->GetDocumentCode()   //3
		;

	transaction->Insert(q.str(params).c_str());

	PtrEnumerator<PrintOption> enumOption = newProfile->EnumOptions();
	while (enumOption.hasNext())
	{
		InsertOption(transaction, userId, profileId, *enumOption);
		enumOption.advance();
	}
	return newProfile.release();
}

void UserDAO::InsertOption(Transaction transaction, uint32_t userId, uint32_t profileId, const PrintOption* option)
{
	SQLQuery q;
	q << L"INSERT INTO  user_print_profile_options ("
		L"option_code,"		//0
		L"profile,"			//1
		L"user_owner,"		//2
		L"option_value"		//3		
		L") VALUES ("
		L"%0q,"
		L"%1,"
		L"%2,"
		L"%3q"
		L")";

	q.parse();

	SQLQueryParms params;

	params << option->name()			//0
		<< profileId					//1
		<< userId						//2
		<< option->get()				//3
		;

	transaction->Insert(q.str(params).c_str());
}


bool UserDAO::DeletePrintProfile(Transaction transaction, uint32_t profileId)
{	
	if (DeleteOptions(transaction, profileId))
	{
		SQLQuery q;
		q << L"DELETE FROM user_print_profiles WHERE profile_id = %0";
		q.parse();
		return transaction->Delete(q.str(profileId).c_str());
	}
	return false;
}

bool UserDAO::LoadSettingDefinitions(std::vector<SettingDefinition*>& result, SimpleProgress *pProgress/* = NULL*/)
{
	SettingDefinition *oneRow = NULL;

	int colSETTINGNAMEPATTERN, colCOLLECTION, colASSOCIATION, colVALUETYPE, colKEYTYPE, colPATTERNORDER;

	Transaction transaction = OpenTransaction();

	int step = 0;
	int steps = transaction->QueryScalar(L"SELECT COUNT(*) FROM user_settings_definitions", 0);
	if (steps < 0) return false;

	SafeSimpleProgress progress(pProgress);

	progress.SetSteps(steps);

	result.clear();

	try
	{
		Cursor cursor = transaction->Query(L"SELECT * FROM user_settings_definitions;");

		colSETTINGNAMEPATTERN = cursor.colIndex(L"SETTING_NAME_PATTERN");
		colCOLLECTION = cursor.colIndex(L"COLLECTION");
		colASSOCIATION = cursor.colIndex(L"ASSOCIATION");
		colVALUETYPE = cursor.colIndex(L"VALUE_TYPE");
		colKEYTYPE = cursor.colIndex(L"KEY_TYPE");
		colPATTERNORDER = cursor.colIndex(L"PATTERN_ORDER");

		while (cursor.nextRow())
		{
			oneRow = new SettingDefinition(cursor.getColString(colSETTINGNAMEPATTERN));

			SettingType type;
			type.SetIsAssociation(cursor.getColInt(colASSOCIATION) != 0);
			type.SetIsCollection(cursor.getColInt(colCOLLECTION) != 0);
			type.SetPropertyType((ValueType)cursor.getColString(colVALUETYPE)[0]);

			std::wstring sKeyType = cursor.getColString(colKEYTYPE);
			if (!sKeyType.empty())
				type.SetKeyType((ValueType)sKeyType[0]);

			oneRow->SetType(type);
			oneRow->SetOrder(cursor.getColInt(colPATTERNORDER));

			result.push_back(oneRow);
			oneRow = NULL;
			if (!progress.Step()) return false;
		}
	}
	catch (SQLException& e)
	{
		if (oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}

bool UserDAO::LoadUserSettings(NewPtrFlushArray<SettingPart>& result, uint32_t userId, SimpleProgress *pProgress/* = NULL*/)
{
	SettingPart *oneRow = NULL;

	int colNAME, colORDER, colVALUE, colKEY;

	SQLQuery countQ;
	countQ << L"SELECT COUNT(*) FROM user_settings WHERE userid = %0";
	countQ.parse();

	Transaction transaction = OpenTransaction();

	int step = 0;
	int steps = transaction->QueryScalar(countQ.str(userId), 0);
	if (steps < 0) return false;

	SafeSimpleProgress progress(pProgress);

	progress.SetSteps(steps);

	result.clear();

	try
	{

		SQLQuery q;
		q << L"SELECT * FROM user_settings WHERE userid = %0";
		q.parse();
		Cursor cursor = transaction->Query(q.str(userId));

		colNAME = cursor.colIndex(L"SETTING_NAME");
		colORDER = cursor.colIndex(L"PART_ORDER");
		colVALUE = cursor.colIndex(L"SETTING_VALUE");
		colKEY = cursor.colIndex(L"ASSOC_KEY");

		while (cursor.nextRow())
		{
			oneRow = new SettingPart(cursor.getColString(colNAME));

			SettingValue value;
			value.SetKey(cursor.getColString(colKEY));
			value.SetValue(cursor.getColString(colVALUE));
			value.SetOrder(cursor.getColInt(colORDER));

			oneRow->SetValue(value);

			result.Add(oneRow);

			oneRow = NULL;
			if (!progress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if (oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}


bool UserDAO::LoadPassRecgntnProfiles(NewPtrFlushArray<PassportRecognitionProfile>& profiles, uint32_t userId, ComplexProgress *pProgress/* = NULL*/)
{
	PassportRecognitionProfile *oneRow = NULL;
	int colPROFILENAME, colPROFILEID, colDIMX, colDIMY;
	PassportField* field = NULL;

	SafeComplexProgress progress(pProgress);
	progress.SetTaskCount(2);
	SafeSimpleProgress fieldsProgress(progress.BeginTask(L"loading_fields"));
	Transaction transaction = OpenTransaction();
	PassportFieldsMap fieldsMap;


	BOOST_SCOPE_EXIT(&fieldsMap)
	{
		DeepDelete(fieldsMap);
	}
	BOOST_SCOPE_EXIT_END


	if (!LoadFields(transaction, fieldsMap, userId, &fieldsProgress))
	{
		return false;
	}

	progress.EndTask();
	SafeSimpleProgress profilesProgress(progress.BeginTask(L"loading_profiles"));

	SQLQuery q;
	q << L"SELECT COUNT(*) FROM user_pass_recgntn_profiles WHERE user_owner = %0";
	q.parse();

	int step = 0;
	int steps = transaction->QueryScalar(q.str(userId), 0);
	if (steps < 0) return false;

	profilesProgress.SetSteps(steps);

	profiles.RemoveAll();

	try
	{
		q.reset();
		q << L"SELECT * FROM user_pass_recgntn_profiles WHERE user_owner = %0";
		q.parse();

		Cursor cursor = transaction->Query(q.str(userId));

		colPROFILENAME = cursor.colIndex(L"profile_name");	
		colPROFILEID = cursor.colIndex(L"profile_id");
		colDIMX = cursor.colIndex(L"dimx");
		colDIMY = cursor.colIndex(L"dimy");

		while (cursor.nextRow())
		{
			oneRow = new PassportRecognitionProfile(cursor.getColInt(colPROFILEID));

			oneRow->SetProfileName(cursor.getColString(colPROFILENAME));
			oneRow->SetXDimention(cursor.getColInt(colDIMX));
			oneRow->SetYDimention(cursor.getColInt(colDIMY));

			auto fieldsRange = fieldsMap.equal_range(oneRow->GetId());
			auto it = fieldsRange.first;
			while (it != fieldsRange.second)
			{
				field = it->second;
				oneRow->SetFieldMetrics(field);
				it++;
			}

			profiles.Add(oneRow);
			oneRow = NULL;
			if (!profilesProgress.Step()) return false;
		}

		progress.EndTask();
	}
	catch (SQLException& e)
	{
		if (oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;

}

bool UserDAO::LoadFields(Transaction transaction, PassportFieldsMap& result, uint32_t userId, SimpleProgress* pProgress)
{
	PassportField *oneRow = NULL;

	int colFIELDNAME, colX, colY, colW, colH, colPROFILEID;
	uint32_t profileId;

	SQLQuery q;
	q << L"SELECT COUNT(*) FROM user_pass_recgntn_profile_fields WHERE user_owner = %0";
	q.parse();

	int step = 0;
	int steps = transaction->QueryScalar(q.str(userId), 0);
	if (steps < 0) return false;

	SafeSimpleProgress progress(pProgress);
	progress.SetSteps(steps);

	try
	{
		q.reset();
		q << L"SELECT * FROM user_pass_recgntn_profile_fields WHERE user_owner = %0";
		q.parse();

		Cursor cursor = transaction->Query(q.str(userId));

		colPROFILEID = cursor.colIndex(L"profile");
		colFIELDNAME = cursor.colIndex(L"field_name");
		
		colX = cursor.colIndex(L"x"); 
		colY = cursor.colIndex(L"y"); 
		colW = cursor.colIndex(L"w"); 
		colH = cursor.colIndex(L"h"); 
		
		PassportFields allFields;
		PassportFieldDescriptor* descriptor;
		std::wstring sFieldName;
		while (cursor.nextRow())
		{
			profileId = cursor.getColInt(colPROFILEID);
			sFieldName = cursor.getColString(colFIELDNAME);

			descriptor = allFields.GetField(sFieldName);
			if (!descriptor)
				BOOST_THROW_EXCEPTION(SQLException(TextUtils::Format(L"Unsupported passport field: %s", sFieldName)));

			oneRow = new PassportField(descriptor);
		
			PassportFieldMetrics metrics(cursor.getColInt(colX),cursor.getColInt(colY),
										 cursor.getColInt(colW),cursor.getColInt(colH));

			oneRow->SetFieldMetrics(metrics);

			result.insert(std::make_pair(profileId, oneRow));

			oneRow = NULL;
			if (!progress.Step()) return false;
		}
	}
	catch (SQLException& e)
	{
		if (oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;

}

void UserDAO::UpdatePassRecgntnProfile(Transaction transaction, IPassportRecognitionProfile* profile)
{
	SQLQuery userQ;
	userQ << L"SELECT user_owner FROM user_pass_recgntn_profiles WHERE profile_id = " << profile->GetId();
	uint32_t userId = transaction->QueryScalar(userQ.str(), NULL_ID);
	DeleteFields(transaction, profile->GetId());

	PtrEnumerator<IPassportField> enumFields = profile->EnumFields();
	while (enumFields.hasNext())
	{
		InsertField(transaction, userId, profile->GetId(), *enumFields);
		enumFields.advance();
	}

	SQLQuery stmt;
	stmt << L"UPDATE user_pass_recgntn_profiles SET "
		L"profile_name = %0q,"			//0
		L"dimx = %1,"					//1
		L"dimy = %2"					//2
		L" WHERE profile_id = %3;";

	stmt.parse();

	SQLQueryParms params;

	params << profile->GetProfileName()			//0
		<< profile->GetXDimention()				//1		
		<< profile->GetYDimention()				//2		
		<< profile->GetId()						//3		
		;

	transaction->Update(stmt.str(params).c_str());


}

bool UserDAO::DeletePassRecgntnProfile(Transaction transaction, uint32_t profileId)
{
	if (DeleteFields(transaction, profileId))
	{
		SQLQuery q;
		q << L"DELETE FROM user_pass_recgntn_profiles WHERE profile_id = %0";
		q.parse();
		return transaction->Delete(q.str(profileId).c_str());
	}
	return false;
}

bool UserDAO::DeleteFields(Transaction transaction, uint32_t profileId)
{
	SQLQuery q;
	q << L"DELETE FROM user_pass_recgntn_profile_fields WHERE profile = %0";
	q.parse();
	return transaction->Delete(q.str(profileId).c_str());
}


PassportRecognitionProfile* UserDAO::InsertPassRecogntnProfile(Transaction transaction, uint32_t userId, IPassportRecognitionProfile* profile)
{
	uint32_t profileId = NextSeqValue(transaction, L"passrecgntnprofiles_seq");

	std::auto_ptr<PassportRecognitionProfile> newProfile(new PassportRecognitionProfile(profileId));
	newProfile->CopyFrom(profile);

	SQLQuery q;
	q << L"INSERT INTO user_pass_recgntn_profiles ("
		L"profile_id,"		//0
		L"profile_name,"	//1
		L"user_owner,"		//2
		L"dimx,"			//3		
		L"dimy"				//4		
		L") VALUES ("
		L"%0,"
		L"%1q,"
		L"%2,"
		L"%3,"
		L"%4"
		L")";

	q.parse();

	SQLQueryParms params;

	params << profileId					//0
		<< newProfile->GetProfileName()	//1
		<< userId						//2
		<< newProfile->GetXDimention()  //3
		<< newProfile->GetYDimention()  //4
		;

	transaction->Insert(q.str(params).c_str());

	PtrEnumerator<IPassportField> enumFields = newProfile->EnumFields();

	while (enumFields.hasNext())
	{
		InsertField(transaction, userId, profileId, *enumFields);
		enumFields.advance();
	}
	return newProfile.release();
}

void UserDAO::InsertField(Transaction transaction, uint32_t userId, uint32_t profileId, IPassportField* field)
{
	SQLQuery q;
	q << L"INSERT INTO  user_pass_recgntn_profile_fields ("
		L"field_name,"		//0
		L"profile,"			//1
		L"user_owner,"		//2
		L"x,"				//3		
		L"y,"				//4		
		L"w,"				//5		
		L"h"				//6		
		L") VALUES ("
		L"%0q,"
		L"%1,"
		L"%2,"
		L"%3,"
		L"%4,"
		L"%5,"
		L"%6"
		L")";

	q.parse();

	SQLQueryParms params;

	PassportFieldMetrics metrics = field->GetFieldMetrics();

	params << field->GetDescriptor()->GetFieldName()	//0
		<< profileId					//1
		<< userId						//2
		<< metrics.GetX()				//3
		<< metrics.GetY()				//4
		<< metrics.GetW()				//5
		<< metrics.GetH()				//6
		;

	transaction->Insert(q.str(params).c_str());
}

bool UserDAO::LoadDeductionDefinitions(NewPtrFlushArray<DeductionDefinition>& result, uint32_t userId, SimpleProgress *pProgress/* = NULL*/)
{
	DeductionDefinition *oneRow = NULL;
	int colDEDUCTIONID, colDEDUCTIONCODE, colFRACTION, colREASON, colALWAYS;
	
	Transaction transaction = OpenTransaction();

	SQLQuery countQ;
	countQ << L"SELECT COUNT(*) FROM user_deduction_definitions WHERE user_id = %0";
	countQ.parse();

	int step = 0;
	int steps = transaction->QueryScalar(countQ.str(userId), 0);
	if (steps < 0) return false;

	SafeSimpleProgress progress(pProgress);

	progress.SetSteps(steps);

	result.RemoveAll();

	try
	{
		SQLQuery q;
		q << L"SELECT * FROM user_deduction_definitions WHERE user_id = %0";
		q.parse();

		Cursor cursor = transaction->Query(q.str(userId));

		colDEDUCTIONID = cursor.colIndex(L"DEDUCTION_ID");
		colDEDUCTIONCODE = cursor.colIndex(L"DEDUCTION_CODE");
		colFRACTION = cursor.colIndex(L"FRACTION");
		colREASON = cursor.colIndex(L"REASON");
		colALWAYS = cursor.colIndex(L"ALWAYS");

		while (cursor.nextRow())
		{
			oneRow = new DeductionDefinition(cursor.getColInt(colDEDUCTIONID));

			oneRow->SetCode(cursor.getColString(colDEDUCTIONCODE));
			oneRow->SetFraction(cursor.getColDecimal(colFRACTION,Decimal(0L)));
			oneRow->SetReason(cursor.getColString(colREASON));
			oneRow->SetIsEnabledByDefault(cursor.getColInt(colALWAYS) != 0);
						
			result.Add(oneRow);
			oneRow = NULL;
			if (!progress.Step()) return false;
		}
	}
	catch (SQLException& e)
	{
		if (oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}

void UserDAO::UpdateDeductionDefinition(Transaction transaction, IDeductionDefinition* deductionDefinition)
{
	SQLQuery stmt;
	stmt << L"UPDATE user_deduction_definitions SET "
		L"deduction_code = %0q,"			//0
		L"fraction = %1,"					//1
		L"reason = %2q,"					//2
		L"always = %3"						//3
		L" WHERE deduction_id = %4;";

	stmt.parse();

	SQLQueryParms params;

	params << deductionDefinition->GetCode()	//0
		<< deductionDefinition->GetFraction()	//1		
		<< deductionDefinition->GetReason()		//2		
		<< (deductionDefinition->GetIsEnabledByDefault() ? 1 : 0)	//3		
		<< deductionDefinition->GetId()			//4
		;

	transaction->Update(stmt.str(params).c_str());
}

DeductionDefinition* UserDAO::InsertDeductionDefinition(Transaction transaction, uint32_t userId, IDeductionDefinition* deductionDefinition)
{
	uint32_t definitionId = NextSeqValue(transaction, L"deductiondefinitions_seq");

	std::auto_ptr<DeductionDefinition> newDefinition(new DeductionDefinition(definitionId));
	newDefinition->CopyFrom(deductionDefinition);

	SQLQuery q;
	q << L"INSERT INTO user_deduction_definitions ("
		L"deduction_id,"	//0
		L"deduction_code,"	//1
		L"fraction,"		//2
		L"reason,"			//3		
		L"always,"			//4
		L"user_id"			//5
		L") VALUES ("
		L"%0,"
		L"%1q,"
		L"%2,"
		L"%3q,"
		L"%4,"
		L"%5"
		L")";

	q.parse();

	SQLQueryParms params;

	params << definitionId				//0
		<< newDefinition->GetCode()		//1
		<< newDefinition->GetFraction()	//2
		<< newDefinition->GetReason()   //3
		<< (newDefinition->GetIsEnabledByDefault() ? 1 : 0) //4
		<< userId //5
		;

	transaction->Insert(q.str(params).c_str());
		
	return newDefinition.release();
}

bool UserDAO::DeleteDeductionDefinition(Transaction transaction, uint32_t deductionId)
{
	SQLQuery q;
	q << L"DELETE FROM user_deduction_definitions WHERE deduction_id = %0";
	q.parse();
	return transaction->Delete(q.str(deductionId).c_str());
}

void UserDAO::InsertSettingPart(Transaction transaction, uint32_t userId, const SettingPart& part)
{
	SQLQuery q;
	q << L"INSERT INTO  user_settings ("
		L"userid,"			//0
		L"setting_name,"	//1
		L"setting_value,"	//2
		L"part_order,"		//3		
		L"assoc_key"		//4				
		L") VALUES ("
		L"%0,"
		L"%1q,"
		L"%2q,"
		L"%3,"
		L"%4q"		
		L")";

	q.parse();

	SQLQueryParms params;

	SettingValue value = part.GetValue();
	params << userId			//0
		<< part.GetName()		//1
		<< value.GetValue()		//2
		<< value.GetOrder()		//3
		<< value.GetKey()		//4		
		;

	transaction->Insert(q.str(params).c_str());
}

bool UserDAO::UpdateSettingPart(Transaction transaction, uint32_t userId, const SettingPart& part)
{
	SQLQuery stmt;
	stmt << L"UPDATE user_settings SET "					
		L"setting_value = %0q,"					//0		
		L"assoc_key = %1q"						//2
		L" WHERE part_order = %2 AND setting_name = %3q AND userid = %4;"; //2 3 4

	stmt.parse();

	SQLQueryParms params;

	SettingValue value = part.GetValue();

	params 	<< value.GetValue()	//0			
		<< value.GetKey()	//1		
		<< value.GetOrder()	//2		
		<< part.GetName()   //3
		<< userId			//4
		;

	return transaction->Update(stmt.str(params).c_str());
}

void UserDAO::DeleteSettingParts(Transaction transaction, uint32_t userId, const std::wstring& settingName)
{
	SQLQuery stmt;
	stmt << L"DELETE FROM user_settings WHERE setting_name = %0q AND userid = %1;";
	stmt.parse();

	transaction->Delete(stmt.str(settingName, userId).c_str());
}