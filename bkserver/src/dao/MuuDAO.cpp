#include "stdafx.h"
#include "MuuDAO.h"
#include <progress/SafeSimpleProgress.h>
#include <progress/SafeComplexProgress.h>
#include "../db/SQLException.h"
#include <data/impl/User.h>
#include "../db/query/SQLQuery.h"
#include <data/impl/InDoc.h>
#include <data/impl/BuyDoc.h>
#include <data/impl/Hent.h>
#include <string/TextUtils.h>
#include "../settings/SettingType.h"
#include <boost/scope_exit.hpp>
#include <arrays/DeepDelete.h>


bool MuuDAO::LoadStocks(NewPtrFlushArray<Stock>& result,uint32_t farmId,SimpleProgress *pProgress/* = NULL*/)
{
	Stock *oneRow = NULL;
		
	int colSTOCKNAME,colSTOCKCODE,colSTOCKID;

	SQLQuery q;
	q << L"SELECT COUNT(*) FROM stocks WHERE farm = %0";
	q.parse();

	Transaction transaction = OpenTransaction();
	
	int step = 0;
	int steps = transaction->QueryScalar(q.str(farmId), 0);
	if(steps < 0) return false;

	SafeSimpleProgress progress(pProgress);

	progress.SetSteps(steps);

	result.RemoveAll();

	try
	{		
		q.reset();
		q << L"SELECT * FROM stocks WHERE farm = %0";
		q.parse();

		Cursor cursor = transaction->Query(q.str(farmId));

		colSTOCKNAME = cursor.colIndex(L"stockname");
		colSTOCKCODE = cursor.colIndex(L"stockcode");
		colSTOCKID = cursor.colIndex(L"stockid");
		
		while (cursor.nextRow())
		{	
			oneRow = new Stock(cursor.getColInt(colSTOCKID));
				
			oneRow->SetStockName(cursor.getColString(colSTOCKNAME));
			oneRow->SetStockCode(cursor.getColString(colSTOCKCODE));
			
			result.Add(oneRow);
			oneRow = NULL;
			if(!progress.Step()) return false;
		}
	}
	catch (SQLException& e)
	{
		if(oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}

bool MuuDAO::LoadClasses(NewPtrFlushArray<Class>& result,uint32_t farmId,SimpleProgress *pProgress/* = NULL*/)
{
	Class *oneRow = NULL;
	
	int colCLASSNAME,colCLASSCODE,colCLASS_ID,colMYPERKGPRICE,colRULE,colPREDEFSEX;

	SQLQuery q;
	q << L"SELECT COUNT(*) FROM classes WHERE farm = %0";
	q.parse();

	Transaction transaction = OpenTransaction();

	int step = 0;
	int steps = transaction->QueryScalar(q.str(farmId), 0);
	if(steps < 0) return false;

	SafeSimpleProgress progress(pProgress);

	progress.SetSteps(steps);

	result.RemoveAll();

	try
	{		
		q.reset();
		q << L"SELECT * FROM classes WHERE farm = %0";
		q.parse();

		Cursor cursor = transaction->Query(q.str(farmId));

		colCLASSNAME = cursor.colIndex(L"classname");
		colCLASSCODE = cursor.colIndex(L"classcode");
		colCLASS_ID = cursor.colIndex(L"class_id");
		colMYPERKGPRICE = cursor.colIndex(L"myperkgprice");
		//colRULE = cursor.colIndex(L"rule");
		colPREDEFSEX = cursor.colIndex(L"predefsex");
				
		while (cursor.nextRow())
		{	
			oneRow = new Class(cursor.getColInt(colCLASS_ID));
				
			oneRow->SetClassName(cursor.getColString(colCLASSNAME));
			oneRow->SetClassCode(cursor.getColString(colCLASSCODE));
			//oneRow->SetMyPerKgPrice(cursor.getColNullDouble(colMYPERKGPRICE));
			//oneRow->SetRule(cursor.getColInt(colRULE));
			oneRow->SetPredefSex(CowSex((COWSEX)cursor.getColInt(colPREDEFSEX, SEX_UNK)));
			
			
			result.Add(oneRow);
			oneRow = NULL;
			if(!progress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}

Farm* MuuDAO::LoadFarm(const FarmNo& farmNo)
{
	Farm *oneRow = NULL;
	int colFARMID, colNAME, colZIP, colCITY, colSTREET, colPOBOX, colFISCALNO, colFARMNO, 
		colWETIDNO, colPHONE, colACCOUNTNO, colBANKNAME, colSTATSNO, colWETLICENCENO, colALIAS;

	Transaction transaction = OpenTransaction();
		
	try
	{		
		Cursor cursor = transaction->Query(L"SELECT * FROM farms where farmNo='" + farmNo.ToString() + L"'");

		colFARMID = cursor.colIndex(L"FARM_ID"); 
		colNAME = cursor.colIndex(L"NAME");
		colALIAS = cursor.colIndex(L"ALIAS");
		colZIP = cursor.colIndex(L"ZIP");
		colCITY = cursor.colIndex(L"CITY");
		colSTREET = cursor.colIndex(L"STREET");
		colPOBOX = cursor.colIndex(L"POBOX");
		colFISCALNO = cursor.colIndex(L"FISCAL_NO");
		colFARMNO = cursor.colIndex(L"FARMNO");
		colWETIDNO = cursor.colIndex(L"WETIDNO");
		colPHONE = cursor.colIndex(L"PHONE");
		colACCOUNTNO = cursor.colIndex(L"ACCOUNTNO");
		colBANKNAME = cursor.colIndex(L"BANKNAME");
		colSTATSNO = cursor.colIndex(L"STATS_NO");
		colWETLICENCENO = cursor.colIndex(L"WETLICENCENO");
				
		if(cursor.nextRow())
		{	
		    oneRow =  new Farm(cursor.getColInt(colFARMID));
			
			oneRow->SetName(cursor.getColString(colNAME));
			oneRow->SetAlias(cursor.getColString(colALIAS));
			oneRow->SetCity(cursor.getColString(colCITY));
			oneRow->SetZipCode(cursor.getColString(colZIP));
			oneRow->SetStreet(cursor.getColString(colSTREET));
			oneRow->SetPOBox(cursor.getColString(colPOBOX));
			oneRow->SetFarmNo(FarmNoFactory::FromString(cursor.getColString(colFARMNO)));	
			oneRow->SetNIP(cursor.getColString(colFISCALNO));
			oneRow->SetREGON(cursor.getColString(colSTATSNO));			
			oneRow->SetWetIdNo(cursor.getColString(colWETIDNO));
			oneRow->SetWetLicNo(cursor.getColString(colWETLICENCENO));
			oneRow->SetPhoneNo(cursor.getColString(colPHONE));			
			
		}
	}
	catch (SQLException e)
	{
		if(oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return oneRow;
}


bool MuuDAO::LoadHerds(NewPtrFlushArray<Herd>& result,IFarm* pFarm,SimpleProgress *pProgress/* = NULL*/)
{
	Herd *oneRow = NULL;

	int colHERDID, colHERDNO, colHERDNAME, colZIP, colCITY, colSTREET, colPOBOX;
		
	Transaction transaction = OpenTransaction();

	int step = 0;
	int steps = transaction->QueryScalar(L"SELECT COUNT(*) FROM herds", 0);
	if(steps < 0) return false;

	SafeSimpleProgress progress(pProgress);

	progress.SetSteps(steps);

	result.RemoveAll();

	try
	{		
		SQLQuery stmt;
		stmt << L"SELECT * FROM herds WHERE farm = %0;";
		stmt.parse();

		Cursor cursor = transaction->Query(stmt.str(pFarm->GetId()));

		colHERDID = cursor.colIndex(L"HERD_ID"); 
		colHERDNO = cursor.colIndex(L"HERDNO"); 
		colHERDNAME = cursor.colIndex(L"HERDNAME"); 
		colZIP = cursor.colIndex(L"ZIP"); 
		colCITY = cursor.colIndex(L"CITY"); 
		colSTREET = cursor.colIndex(L"STREET"); 
		colPOBOX = cursor.colIndex(L"POBOX"); 
		//colLONGITUDE = cursor.colIndex(L"LONGITUDE"); 
		//colLATITUDE = cursor.colIndex(L"LATITUDE"); 
		
						
		while(cursor.nextRow())
		{	
		    oneRow =  new Herd(cursor.getColInt(colHERDID));
			
			oneRow->SetHerdIndex(cursor.getColInt(colHERDNO));
			oneRow->SetHerdName(cursor.getColString(colHERDNAME));
			oneRow->SetCity(cursor.getColString(colCITY));
			oneRow->SetZipCode(cursor.getColString(colZIP));
			oneRow->SetStreet(cursor.getColString(colSTREET));
			oneRow->SetPOBox(cursor.getColString(colPOBOX));
			
			//oneRow->setL
					
			result.Add(oneRow);
			oneRow = NULL;
			if(!progress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}

void MuuDAO::UpdateHerd(Transaction transaction,const IHerd* pHerd)
{
}

void MuuDAO::DeleteHerd(Transaction transaction,const IHerd* pHerd)
{
}

Herd* MuuDAO::InsertHerd(Transaction transaction,const IHerd* pHerd)
{
	return NULL;
}

bool MuuDAO::LoadSeasons(NewPtrFlushArray<Season>& result,IFarm* pFarm,SimpleProgress *pProgress/* = NULL*/)
{
	Season *oneRow = NULL;

	int colSEASONID, colSEASONNAME, colSTARTDATE, colENDDATE;
			
	Transaction transaction = OpenTransaction();

	int step = 0;
	int steps = transaction->QueryScalar(L"SELECT COUNT(*) FROM seasons",0);
	if(steps < 0) return false;

	SafeSimpleProgress progress(pProgress);

	progress.SetSteps(steps);

	result.RemoveAll();

	try
	{		
		SQLQuery stmt;
		stmt << L"SELECT * FROM seasons WHERE farm = %0;";
		stmt.parse();

		Cursor cursor = transaction->Query(stmt.str(pFarm->GetId()));

		colSEASONID = cursor.colIndex(L"SEASON_ID"); 
		colSEASONNAME = cursor.colIndex(L"SEASON_NAME"); 
		colSTARTDATE = cursor.colIndex(L"START_DATE"); 
		colENDDATE = cursor.colIndex(L"END_DATE"); 
		 	
						
		while(cursor.nextRow())
		{	
			oneRow =  new Season(cursor.getColInt(colSEASONID));
			oneRow->SetSeasonName(cursor.getColString(colSEASONNAME));
			oneRow->SetStartDate(cursor.getColDateTime(colSTARTDATE));
			oneRow->SetEndDate(cursor.getColDateTime(colENDDATE));
								
			result.Add(oneRow);
			oneRow = NULL;
			if(!progress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}

void MuuDAO::UpdateSeason(Transaction transaction,const ISeason* pSeason)
{

}

void MuuDAO::DeleteSeason(Transaction transaction,const ISeason* pSeason)
{
}

Season* MuuDAO::InsertSeason(Transaction transaction,const ISeason* pSeason)
{
	return NULL;
}

bool MuuDAO::LoadUsers(NewPtrFlushArray<User>& result,IFarm* pFarm,SimpleProgress *pProgress/* = NULL*/)
{
	User *oneRow = NULL;

	int colUSERNAME,colUSERID,colFNAME,colLNAME,colPASS,colLASTLOGIN;

	Transaction transaction = OpenTransaction();

	SQLQuery q;
	q << L"SELECT COUNT(*) FROM users WHERE farm = %0";
	q.parse();

	int step = 0;
	int steps = transaction->QueryScalar(q.str(pFarm->GetId()),0);
	if(steps < 0) return false;

	SafeSimpleProgress progress(pProgress);
	progress.SetSteps(steps);

	result.RemoveAll();

	try
	{
		
		SQLQuery stmt;
		stmt << "SELECT * FROM users WHERE farm = %0;";
		stmt.parse();

		Cursor cursor = transaction->Query(stmt.str(pFarm->GetId()));
	
		colUSERNAME = cursor.colIndex(L"USERNAME");
		colUSERID = cursor.colIndex(L"USERID");
		colFNAME = cursor.colIndex(L"FNAME");
		colLNAME = cursor.colIndex(L"LNAME");
		colPASS = cursor.colIndex(L"PASS");
		colLASTLOGIN = cursor.colIndex(L"LASTLOGIN");
			
		while(cursor.nextRow())
		{	
			oneRow = new User(cursor.getColInt(colUSERID),pFarm);
			oneRow->SetFirstName(cursor.getColString(colFNAME));
			oneRow->SetLastName(cursor.getColString(colLNAME));
			oneRow->SetPassword(cursor.getColString(colPASS));
			oneRow->SetLogin(cursor.getColString(colUSERNAME));
			oneRow->SetLastLogin(cursor.getColDateTime(colLASTLOGIN));			
			result.Add(oneRow);
			oneRow = NULL;
			if(!progress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}

void MuuDAO::UpdateUser(Transaction transaction,const IUser* pUser)
{
}

void MuuDAO::DeleteUser(Transaction transaction,const IUser* pSeason)
{
}

User* MuuDAO::InsertUser(Transaction transaction,const IUser* pSeason)
{
	return NULL;
}


bool MuuDAO::UpdateHent(Transaction transaction,IHent* pHent)
{
	SQLQuery q;

	q <<	L"UPDATE hents SET "
			L"alias = %0q," 
			L"name = %1q,"
			L"zip = %2q," 
			L"city = %3q,"
			L"street = %4q,"
			L"pobox = %5q," 
			L"fiscal_no = %6n,"
			L"hentno = %7q," 
			L"wetno = %8n,"
			L"phone_no = %9n," 
			L"plate_no = %10n," 
			L"extras = %11n,"
			L"accountno = %12n,"
			L"bankname = %13n," 
			L"personal_no = %14n," 
			L"stats_no = %15n," 
			L"personal_id_no = %16n,"
			L"issuepost = %17n,"
			L"issuedate = %18n," 
			L"syncit = %19,"
			L"cellphone_no = %20n,"
			L"email = %21n,"
			L"latitude = %22,"
			L"longitude = %23,"
			L"wetlicenceno = %24n,"
			L"henttype = %25q"
			L" WHERE hentid = %26";
	q.parse();

	SQLQueryParms params;

	params << pHent->GetAlias()					//0
		  << pHent->GetName()					//1
		  << pHent->GetZip()					//2
		  << pHent->GetCity()					//3
		  << pHent->GetStreet()					//4
		  << pHent->GetPOBox()					//5
		  << pHent->GetFiscalNo()				//6
		  << pHent->GetHentNo()					//7
		  << pHent->GetWetNo()					//8
		  << pHent->GetPhoneNo()				//9
		  << pHent->GetPlateNo()				//10
		  << pHent->GetExtras()					//11
		  << pHent->GetAccountNo()				//12
		  << pHent->GetBankName()				//13
		  << pHent->GetPESEL()					//14
		  << pHent->GetREGON()					//15
		  << pHent->GetIdNo()					//16
		  << pHent->GetIssuePost()				//17
		  << SQLString(pHent->GetIssueDate(),true)	//18
		  << (pHent->GetSyncIt() ? 1 : 0)		//19
		  << pHent->GetCellPhoneNo()			//20
		  << pHent->GetEmailAddress()			//21
		  << pHent->GetLatitude()				//22
		  << pHent->GetLongitude()				//23
		  << pHent->GetWetLicenceNo()			//24
		  << pHent->GetHentType()				//25
		  << pHent->GetId();					//26

	return transaction->Update(q.str(params).c_str());	
}

bool MuuDAO::DeleteHent(Transaction transaction,IHent* pHent)
{
	SQLQuery q;

	q << L"DELETE FROM hents where hentId = %0";
	q.parse();

	return transaction->Delete(q.str(pHent->GetId()).c_str());
}

Hent* MuuDAO::InsertHent(Transaction transaction,uint32_t season,IHent* pHent)
{		
	SQLQuery q;

	q <<  L"INSERT INTO hents(name,alias,street,pobox,city,zip,phone_no,fiscal_no,hentno,wetno,extras,plate_no,"
							L"henttype,accountno,bankname,personal_no,stats_no,personal_id_no,issuepost,issuedate,cellphone_no,"
							L"email,latitude,longitude,wetlicenceno,season,hentid)"
							L" VALUES (%0q,%1q,%2n,%3n,%4n,%5n,%6n,%7n,%8n,%9n,%10n,%11n,%12q,%13n,%14n,%15n,%16n,"
							L"%17n,%18n,%19n,%20n,%21n,%22,%23,%24n,%25,%26);";
	q.parse();
	SQLQueryParms params;

	uint32_t hentId = NextSeqValue(transaction,L"hents_seq");

	params << pHent->GetName()					//0
		  << pHent->GetAlias()					//1
		  << pHent->GetStreet()					//2
		  << pHent->GetPOBox()					//3
		  << pHent->GetCity()					//4
		  << pHent->GetZip()					//5
		  << pHent->GetPhoneNo()				//6
		  << pHent->GetFiscalNo()				//7
		  << pHent->GetHentNo()					//8
		  << pHent->GetWetNo()					//9
		  << pHent->GetExtras()		  			//10
		  << pHent->GetPlateNo()				//11
		  << pHent->GetHentType()			//12
		  << pHent->GetAccountNo()				//13
		  << pHent->GetBankName()				//14
		  << pHent->GetPESEL()					//15
		  << pHent->GetREGON()					//16
		  << pHent->GetIdNo()					//17
		  << pHent->GetIssuePost()				//18
		  << SQLString(pHent->GetIssueDate(),true)		//19
		  << pHent->GetCellPhoneNo()			//20
		  << pHent->GetEmailAddress()			//21
		  << pHent->GetLatitude()				//22
		  << pHent->GetLongitude()				//23
		  << pHent->GetWetLicenceNo()			//24
		  << season								//25
		  << hentId								//26
		  ;
		  
	transaction->Insert(q.str(params).c_str());
	
	Hent* pNewHent = new Hent(hentId);
	pNewHent->CopyFrom(pHent);
	return pNewHent;
		
}


bool MuuDAO::LoadHents(NewPtrFlushArray<Hent>& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	Hent *oneHent = NULL;

	int colHENTNO,colALIAS,colCITY,colHENTID,colEXTRAS,colNAME,colNIP,colPHONE,
		colPOBOX,colSTREET,colWETNO,colZIP,colPLATE,colHENTTYPE,colPESEL,colREGON,colIDNO,
		colISSUEDATE,colISSUEPOST,colACCOUNTNO,colBANKNAME,colSYNCIT,
		colWETLICENCENO,colCELLPHONENO,colEMAILADDRESS,colLATITUDE,colLONGITUDE;
		
	SQLQuery q;

	Transaction transaction = OpenTransaction();

	int step = 0;
	q << L"SELECT COUNT(*) FROM hents WHERE season = %0";
	q.parse();

	int steps = transaction->QueryScalar(q.str(seasonId),0);
	if(steps < 0) return false;
	SafeSimpleProgress progress(pProgress);

	progress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM hents WHERE season = %0";
	q.parse();

	
	try
	{	
		Cursor cursor = transaction->Query(q.str(seasonId));

		colHENTNO = cursor.colIndex(L"hentno");
	    colALIAS = cursor.colIndex(L"alias");
		colCITY = cursor.colIndex(L"city");
		colHENTID = cursor.colIndex(L"hentid");
		colEXTRAS = cursor.colIndex(L"extras");
		colNAME = cursor.colIndex(L"name");
		colNIP = cursor.colIndex(L"fiscal_no");
		colPHONE = cursor.colIndex(L"phone_no");
		colPOBOX = cursor.colIndex(L"pobox");
		colSTREET = cursor.colIndex(L"street");
		colWETNO = cursor.colIndex(L"wetno");
		colZIP = cursor.colIndex(L"zip");
		colPLATE = cursor.colIndex(L"plate_no");
		colHENTTYPE = cursor.colIndex(L"henttype");
		colPESEL = cursor.colIndex(L"personal_no");
		colREGON = cursor.colIndex(L"stats_no");
		colIDNO = cursor.colIndex(L"personal_id_no");
		colISSUEDATE = cursor.colIndex(L"issuedate");
		colISSUEPOST = cursor.colIndex(L"issuepost");
		colACCOUNTNO = cursor.colIndex(L"accountno");
		colBANKNAME = cursor.colIndex(L"bankname");
		colSYNCIT = cursor.colIndex(L"syncit");
		colWETLICENCENO  = cursor.colIndex(L"wetlicenceno");
		colCELLPHONENO = cursor.colIndex(L"cellphone_no");
		colEMAILADDRESS = cursor.colIndex(L"email");
		colLATITUDE = cursor.colIndex(L"latitude");
		colLONGITUDE = cursor.colIndex(L"longitude");
        		
		while(cursor.nextRow())
		{			
			uint32_t hentId = cursor.getColInt(colHENTID);
			oneHent = new Hent(hentId);
			
			std::wstring sHentType = cursor.getColString(colHENTTYPE);
			oneHent->SetHentType((HentType)sHentType[0]);			

			std::wstring sHentNo = cursor.getColString(colHENTNO);
			try
			{
				oneHent->SetHentNo(HentNoFactory::FromString(sHentNo));
			}
			catch (HentNoException& e)
			{
				throw SQLException(TextUtils::Format(L"invalid hentno '%s' for hent with id = %d: %s", sHentNo, hentId,e.wwhat()));
			}

			oneHent->SetAlias(cursor.getColString(colALIAS));
			oneHent->SetCity(cursor.getColString(colCITY));
			oneHent->SetExtras(cursor.getColString(colEXTRAS));
			oneHent->SetName(cursor.getColString(colNAME));
			oneHent->SetFiscalNo(cursor.getColString(colNIP));
			oneHent->SetPhoneNo(cursor.getColString(colPHONE));
			oneHent->SetPOBox(cursor.getColString(colPOBOX));
			oneHent->SetStreet(cursor.getColString(colSTREET));
			oneHent->SetWetNo(cursor.getColString(colWETNO));
			oneHent->SetZip(cursor.getColString(colZIP));
			oneHent->SetPlateNo(cursor.getColString(colPLATE));			
			oneHent->SetPESEL(cursor.getColString(colPESEL));
			oneHent->SetREGON(cursor.getColString(colREGON));
			oneHent->SetIdNo(cursor.getColString(colIDNO));
			oneHent->SetIssueDate(cursor.getColDateTime(colISSUEDATE));
			oneHent->SetIssuePost(cursor.getColString(colISSUEPOST));


			std::wstring sAccountNo = cursor.getColString(colACCOUNTNO);
			try
			{
				oneHent->SetAccountNo(AccountNoFactory::FromString(sAccountNo));
			}
			catch (AccountNoException& e)
			{
				throw SQLException(TextUtils::Format(L"invalid accountno '%s' for hent with id = %d: %s", sAccountNo, hentId, e.wwhat()));
			}
						
			oneHent->SetBankName(cursor.getColString(colBANKNAME));
			oneHent->SetSyncIt(cursor.getColInt(colSYNCIT) > 0);
			oneHent->SetWetLicenceNo(cursor.getColString(colWETLICENCENO));
			oneHent->SetCellPhoneNo(cursor.getColString(colCELLPHONENO));
			oneHent->SetEmailAddress(cursor.getColString(colEMAILADDRESS));
			oneHent->SetLatitude(Latitude(cursor.getColNullDouble(colLATITUDE)));
			oneHent->SetLongitude(Longitude(cursor.getColNullDouble(colLONGITUDE)));

			result.Add(oneHent);
			oneHent = NULL;
			step++;
			if(!progress.Step()) return false;
	    }	
	}
	catch (SQLException& e)
	{
		if(oneHent) delete oneHent;
		BOOST_THROW_EXCEPTION(e);
	}

	return true;

}

bool MuuDAO::LoadCows(NewPtrFlushArray<Cow>& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	Cow *onecow = NULL;
	
	int colANIMALID,colCOWNO,colBIRTHDATE,colBIRTHPLACE,				
		colSEX,colSTOCK,colCLASS, colMOTHERNO, colWEIGHT, colPASSNO, colPASSDATE,
		colFSTOWNR, colEXTRAS,
		colTERMBUYCLASS,colTERMBUYWEIGHT,colTERMSELLCLASS,colTERMSELLWEIGHT,colTERMBUYPRICE,
		colTERMSELLPRICE,colPASSIMGLABEL,colBUYINVOICE,colSELLINVOICE,colHEALTHCERTNO;

	SQLQuery q;
	
	int step = 0;
	q << L"SELECT COUNT(*) FROM cattle WHERE season = %0";
	q.parse();

	Transaction transaction = OpenTransaction();

	int steps = transaction->QueryScalar(q.str(seasonId),0);
	if(steps < 0) return false;
	SafeSimpleProgress progress(pProgress);

	progress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM cattle WHERE season = %0";
	q.parse();


	try
	{
		Cursor cursor = transaction->Query(q.str(seasonId));

		colANIMALID =  cursor.colIndex(L"ANIMALID");
		colCOWNO = cursor.colIndex(L"COWNO");
		colBIRTHDATE = cursor.colIndex(L"BIRTHDATE");
		colBIRTHPLACE =	cursor.colIndex(L"BIRTHPLACE");				
		colSEX = cursor.colIndex(L"SEX");
		colSTOCK = cursor.colIndex(L"STOCK");
		colCLASS = cursor.colIndex(L"CLASS");
		colMOTHERNO = cursor.colIndex(L"MOTHERNO");
		colWEIGHT = cursor.colIndex(L"WEIGHT");
		colPASSNO =	cursor.colIndex(L"PASSNO");
		colPASSDATE = cursor.colIndex(L"passdate");
		colFSTOWNR = cursor.colIndex(L"fstownr");
		colEXTRAS = cursor.colIndex(L"extras");
		colTERMBUYCLASS = cursor.colIndex(L"TERMBUYCLASS");
		colTERMBUYWEIGHT = cursor.colIndex(L"TERMBUYWEIGHT");
		colTERMSELLCLASS = cursor.colIndex(L"TERMSELLCLASS");
		colTERMSELLWEIGHT = cursor.colIndex(L"TERMSELLWEIGHT");
		colTERMBUYPRICE = cursor.colIndex(L"TERMBUYPRICE");
		colTERMSELLPRICE = cursor.colIndex(L"TERMSELLPRICE");
		colPASSIMGLABEL = cursor.colIndex(L"PASSIMGLABEL");
		colBUYINVOICE = cursor.colIndex(L"BUY_INVOICE");
		colSELLINVOICE = cursor.colIndex(L"SELL_INVOICE");
		colHEALTHCERTNO  = cursor.colIndex(L"HEALTHCERTNO");
		
		while(cursor.nextRow())
		{
			onecow = new Cow(cursor.getColInt(colANIMALID));			
			onecow->SetCowNo(CowNoFactory::FromString(cursor.getColString(colCOWNO)));
			onecow->SetBirthDate(cursor.getColDateTime(colBIRTHDATE));
			onecow->SetBirthPlace(cursor.getColString(colBIRTHPLACE));				
			onecow->SetSex(CowSex((COWSEX)cursor.getColInt(colSEX)));         				
			onecow->SetStockId(cursor.getColInt(colSTOCK));				
			onecow->SetClassId(cursor.getColInt(colCLASS,NULL_ID));
			onecow->SetMotherNo(CowNoFactory::FromString(cursor.getColString(colMOTHERNO)));
			onecow->SetWeight(cursor.getColDecimal(colWEIGHT,Decimal(0L)));
			onecow->SetPassNo(cursor.getColString(colPASSNO));
			onecow->SetPassDate(cursor.getColDateTime(colPASSDATE));			
			onecow->SetFirstOwnerId(cursor.getColInt(colFSTOWNR));
			onecow->SetExtras(cursor.getColString(colEXTRAS));		
			onecow->SetTermBuyClassId(cursor.getColInt(colTERMBUYCLASS));
			onecow->SetTermSellClassId(cursor.getColInt(colTERMSELLCLASS));
			onecow->SetTermBuyWeight(cursor.getColNullDecimal(colTERMBUYWEIGHT));
			onecow->SetTermSellWeight(cursor.getColNullDecimal(colTERMSELLWEIGHT));
			onecow->SetTermBuyPrice(cursor.getColNullDecimal(colTERMBUYPRICE));
			onecow->SetTermSellPrice(cursor.getColNullDecimal(colTERMSELLPRICE));
			onecow->SetPassImgLabel(cursor.getColString(colPASSIMGLABEL));
			onecow->SetHealthCertNo(cursor.getColString(colHEALTHCERTNO));
			onecow->SetSellInvoiceId(cursor.getColInt(colSELLINVOICE));
			onecow->SetBuyInvoiceId(cursor.getColInt(colBUYINVOICE));
			result.Add(onecow);						
			
			onecow = NULL;

			step++;
			if(!progress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(onecow) delete onecow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}

bool MuuDAO::LoadInDocs(NewPtrFlushArray<InDoc>& result,uint32_t seasonId,ComplexProgress *pProgress/* = NULL*/)
{	
	InDoc *oneDoc = NULL;
	
	int colDOCID,colDOCDATE,colEXTRAS,colPLATENO,colLOADDATE,colLOADENDTM,colLOADSTRTM,colAGENT,colMOTIVE,colHERD;	

	SQLQuery q;
		
	SafeComplexProgress progress(pProgress);

	SimpleProgress* pLoadingEntriesProgress = progress.BeginTask(L"loading_indocs_entries");
	std::multimap<uint32_t,CowEntry*> entries;
	
	Transaction transaction = OpenTransaction();

	if(!LoadInDocCows(entries,seasonId,pLoadingEntriesProgress))
		return false;

	progress.EndTask();
	
	SafeSimpleProgress loadingInDocsProgress(progress.BeginTask(L"loading_indocs"));
	int step = 0;
	q << L"SELECT COUNT(*) FROM indocs WHERE season = %0";
	q.parse();

	int steps = transaction->QueryScalar(q.str(seasonId),0);
	if(steps < 0) return false;
	loadingInDocsProgress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM indocs WHERE season = %0";
	q.parse();

	try
	{
		Cursor cursor = transaction->Query(q.str(seasonId));

		colDOCID =		cursor.colIndex(L"DOCID");
		colDOCDATE =	cursor.colIndex(L"DOCDATE");
		colEXTRAS =		cursor.colIndex(L"EXTRAS");		
		colPLATENO =	cursor.colIndex(L"PLATENO");
		colLOADDATE =	cursor.colIndex(L"LOADDATE");
		colLOADENDTM =	cursor.colIndex(L"LOADENDTM");
		colLOADSTRTM =	cursor.colIndex(L"LOADENDTM");
		colAGENT =		cursor.colIndex(L"AGENT");
		colMOTIVE =		cursor.colIndex(L"MOTIVE");
		colHERD =		cursor.colIndex(L"HERD");
				
		while(cursor.nextRow())
		{
			oneDoc = new InDoc(cursor.getColInt(colDOCID));
							
			oneDoc->SetAgentId(cursor.getColInt(colAGENT,NULL_ID));
			oneDoc->SetDocDate(cursor.getColDateTime(colDOCDATE));
			oneDoc->SetExtras(cursor.getColString(colEXTRAS));
			oneDoc->SetLoadEndDtTm(Time(cursor.getColNullInt(colLOADENDTM)));
			oneDoc->SetLoadStartDtTm(Time(cursor.getColNullInt(colLOADSTRTM)));
			oneDoc->SetHerdId(cursor.getColInt(colHERD));		
			oneDoc->SetLoadDate(cursor.getColDateTime(colLOADDATE));

			std::wstring sMotive = cursor.getColString(colMOTIVE);
			oneDoc->SetMotive((Motive)sMotive[0]);
			oneDoc->SetPlateNo(cursor.getColString(colPLATENO));
			
			std::pair <EntriesMap::iterator, EntriesMap::iterator> entriesRange = entries.equal_range(oneDoc->GetId());
			EntriesMap::iterator entryIt = entriesRange.first;
			while(entryIt != entriesRange.second)
			{
				oneDoc->AddCow(entryIt->second);
				entryIt++;
			}

			result.Add(oneDoc);

			oneDoc = NULL;
			step++;
			if(!loadingInDocsProgress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneDoc) delete oneDoc;
		BOOST_THROW_EXCEPTION(e);
	}
	progress.EndTask();
	return true;	
}


bool MuuDAO::LoadBuyDocs(NewPtrFlushArray<BuyDoc>& result,uint32_t seasonId,ComplexProgress *pProgress/* = NULL*/)
{
	BuyDoc *oneDoc = NULL;
	
	int colDOCID,colDOCDATE,colEXTRAS,colPLATENO,colLOADDATE,colLOADENDTM,colLOADSTRTM,colAGENT,colMOTIVE,colHERD,colHENT,colINVOICE;	

	SQLQuery q;
	
	Transaction transaction = OpenTransaction();

	SafeComplexProgress progress(pProgress);

	SimpleProgress* pLoadingEntriesProgress = progress.BeginTask(L"loading_buydocs_entries");
	std::multimap<uint32_t,CowEntry*> entries;
	
	if(!LoadBuyDocCows(entries,seasonId,pLoadingEntriesProgress))
		return false;

	progress.EndTask();
	
	SafeSimpleProgress loadingBuyDocsProgress(progress.BeginTask(L"loading_buydocs"));
	int step = 0;
	q << L"SELECT COUNT(*) FROM buydocs WHERE season = %0";
	q.parse();

	int steps = transaction->QueryScalar(q.str(seasonId),0);
	if(steps < 0) return false;
	loadingBuyDocsProgress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM buydocs WHERE season = %0";
	q.parse();

	try
	{
		Cursor cursor = transaction->Query(q.str(seasonId));

		colDOCID =		cursor.colIndex(L"DOCID");
		colDOCDATE =	cursor.colIndex(L"DOCDATE");
		colEXTRAS =		cursor.colIndex(L"EXTRAS");		
		colPLATENO =	cursor.colIndex(L"PLATENO");
		colLOADDATE =	cursor.colIndex(L"LOADDATE");
		colLOADENDTM =	cursor.colIndex(L"LOADENDTM");
		colLOADSTRTM =	cursor.colIndex(L"LOADENDTM");
		colAGENT =		cursor.colIndex(L"AGENT");
		colMOTIVE =		cursor.colIndex(L"MOTIVE");
		colHERD =		cursor.colIndex(L"HERD");
		colHENT =		cursor.colIndex(L"HENT");
		colINVOICE =	cursor.colIndex(L"INVOICE");
				
		while(cursor.nextRow())
		{
			oneDoc = new BuyDoc(cursor.getColInt(colDOCID));
								
			oneDoc->SetAgentId(cursor.getColInt(colAGENT,NULL_ID));
			oneDoc->SetDocDate(cursor.getColDateTime(colDOCDATE));
			oneDoc->SetExtras(cursor.getColString(colEXTRAS));
			oneDoc->SetLoadDate(cursor.getColDateTime(colLOADDATE));
			oneDoc->SetLoadEndDtTm(Time(cursor.getColNullInt(colLOADENDTM)));
			oneDoc->SetLoadStartDtTm(Time(cursor.getColNullInt(colLOADSTRTM)));
			oneDoc->SetHerdId(cursor.getColInt(colHERD,NULL_ID));
			oneDoc->SetHentId(cursor.getColInt(colHENT,NULL_ID));
			oneDoc->SetInvoiceId(cursor.getColInt(colINVOICE,NULL_ID));

			std::wstring sMotive = cursor.getColString(colMOTIVE);
			oneDoc->SetMotive((Motive)sMotive[0]);
			oneDoc->SetPlateNo(cursor.getColString(colPLATENO));
			
			std::pair <EntriesMap::iterator, EntriesMap::iterator> entriesRange = entries.equal_range(oneDoc->GetId());
			EntriesMap::iterator entryIt = entriesRange.first;
			while(entryIt != entriesRange.second)
			{
				oneDoc->AddCow(entryIt->second);
				entryIt++;
			}

			result.Add(oneDoc);

			oneDoc = NULL;
			step++;
			if(!loadingBuyDocsProgress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneDoc) delete oneDoc;
		BOOST_THROW_EXCEPTION(e);
	}
	progress.EndTask();
	return true;
}

bool MuuDAO::LoadMoveDocs(NewPtrFlushArray<MoveDoc>& result,uint32_t seasonId,ComplexProgress *pProgress/* = NULL*/)
{
	MoveDoc *oneDoc = NULL;
	
	int colDOCID,colDOCDATE,colEXTRAS,colPLATENO,colLOADDATE,colLOADENDTM,colLOADSTRTM,colAGENT,colHERDFROM,colHERDTO;	

	SQLQuery q;
	Transaction transaction = OpenTransaction();
	
	SafeComplexProgress progress(pProgress);

	SimpleProgress* pLoadingEntriesProgress = progress.BeginTask(L"loading_movedocs_entries");
	std::multimap<uint32_t,CowEntry*> entries;
	
	if(!LoadMoveDocCows(entries,seasonId,pLoadingEntriesProgress))
		return false;

	progress.EndTask();
	
	SafeSimpleProgress loadingMoveDocsProgress(progress.BeginTask(L"loading_movedocs"));
	int step = 0;
	q << L"SELECT COUNT(*) FROM movedocs WHERE season = %0";
	q.parse();

	int steps = transaction->QueryScalar(q.str(seasonId),0);
	if(steps < 0) return false;
	loadingMoveDocsProgress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM movedocs WHERE season = %0 ORDER BY docid";
	q.parse();

	try
	{
		Cursor cursor = transaction->Query(q.str(seasonId));

		colDOCID =		cursor.colIndex(L"DOCID");
		colDOCDATE =	cursor.colIndex(L"DOCDATE");
		colEXTRAS =		cursor.colIndex(L"EXTRAS");		
		colPLATENO =	cursor.colIndex(L"PLATENO");
		colLOADDATE =	cursor.colIndex(L"LOADDATE");
		colLOADENDTM =	cursor.colIndex(L"LOADENDTM");
		colLOADSTRTM =	cursor.colIndex(L"LOADENDTM");
		colAGENT =		cursor.colIndex(L"AGENT");
		colHERDFROM =	cursor.colIndex(L"HERD_FROM");
		colHERDTO	=	cursor.colIndex(L"HERD_TO");
				
		while(cursor.nextRow())
		{
			oneDoc = new MoveDoc(cursor.getColInt(colDOCID));
							
			oneDoc->SetAgentId(cursor.getColInt(colAGENT,NULL_ID));
			oneDoc->SetDocDate(cursor.getColDateTime(colDOCDATE));
			oneDoc->SetExtras(cursor.getColString(colEXTRAS));
			oneDoc->SetLoadDate(cursor.getColDateTime(colLOADDATE));
			oneDoc->SetLoadEndDtTm(Time(cursor.getColNullInt(colLOADENDTM)));
			oneDoc->SetLoadStartDtTm(Time(cursor.getColNullInt(colLOADSTRTM)));	
			oneDoc->SetPlateNo(cursor.getColString(colPLATENO));
			oneDoc->SetSrcHerdId(cursor.getColInt(colHERDFROM));		
			oneDoc->SetDstHerdId(cursor.getColInt(colHERDTO));		
			
			std::pair <EntriesMap::iterator, EntriesMap::iterator> entriesRange = entries.equal_range(oneDoc->GetId());
			EntriesMap::iterator entryIt = entriesRange.first;
			while(entryIt != entriesRange.second)
			{
				oneDoc->AddCow(entryIt->second);
				entryIt++;
			}

			result.Add(oneDoc);

			oneDoc = NULL;
			step++;
			if(!loadingMoveDocsProgress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneDoc) delete oneDoc;
		BOOST_THROW_EXCEPTION(e);
	}
	progress.EndTask();
	return true;
}

bool MuuDAO::LoadSellDocs(NewPtrFlushArray<SellDoc>& result,uint32_t seasonId,ComplexProgress *pProgress/* = NULL*/)
{
	SellDoc *oneDoc = NULL;
	
	int colDOCID,colDOCDATE,colEXTRAS,colPLATENO,colLOADDATE,colLOADENDTM,colLOADSTRTM,colAGENT,colMOTIVE,colHERD,colHENT,colINVOICE;	

	SQLQuery q;

	Transaction transaction = OpenTransaction();
	
	SafeComplexProgress progress(pProgress);

	SimpleProgress* pLoadingEntriesProgress = progress.BeginTask(L"loading_selldocs_entries");
	std::multimap<uint32_t,CowEntry*> entries;
	
	if(!LoadSellDocCows(entries,seasonId,pLoadingEntriesProgress))
		return false;

	progress.EndTask();
	
	SafeSimpleProgress loadingSellDocsProgress(progress.BeginTask(L"loading_selldocs"));
	int step = 0;
	q << L"SELECT COUNT(*) FROM selldocs WHERE season = %0";
	q.parse();

	int steps = transaction->QueryScalar(q.str(seasonId),0);
	if(steps < 0) return false;
	loadingSellDocsProgress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM selldocs WHERE season = %0";
	q.parse();

	try
	{
		Cursor cursor = transaction->Query(q.str(seasonId));

		colDOCID =		cursor.colIndex(L"DOCID");
		colDOCDATE =	cursor.colIndex(L"DOCDATE");
		colEXTRAS =		cursor.colIndex(L"EXTRAS");		
		colPLATENO =	cursor.colIndex(L"PLATENO");
		colLOADDATE =	cursor.colIndex(L"LOADDATE");
		colLOADENDTM =	cursor.colIndex(L"LOADENDTM");
		colLOADSTRTM =	cursor.colIndex(L"LOADENDTM");
		colAGENT =		cursor.colIndex(L"AGENT");
		colMOTIVE =		cursor.colIndex(L"MOTIVE");
		colHERD =		cursor.colIndex(L"HERD");
		colHENT =		cursor.colIndex(L"HENT");
		colINVOICE =	cursor.colIndex(L"INVOICE");
				
		while(cursor.nextRow())
		{
			oneDoc = new SellDoc(cursor.getColInt(colDOCID));
								
			oneDoc->SetAgentId(cursor.getColInt(colAGENT,NULL_ID));
			oneDoc->SetDocDate(cursor.getColDateTime(colDOCDATE));
			oneDoc->SetExtras(cursor.getColString(colEXTRAS));
			oneDoc->SetLoadDate(cursor.getColDateTime(colLOADDATE));
			oneDoc->SetLoadEndDtTm(Time(cursor.getColNullInt(colLOADENDTM)));
			oneDoc->SetLoadStartDtTm(Time(cursor.getColNullInt(colLOADSTRTM)));
			oneDoc->SetHerdId(cursor.getColInt(colHERD,NULL_ID));
			oneDoc->SetHentId(cursor.getColInt(colHENT,NULL_ID));
			oneDoc->SetInvoiceId(cursor.getColInt(colINVOICE,NULL_ID));

			std::wstring sMotive = cursor.getColString(colMOTIVE);
			oneDoc->SetMotive((Motive)sMotive[0]);
			oneDoc->SetPlateNo(cursor.getColString(colPLATENO));
			
			std::pair <EntriesMap::iterator, EntriesMap::iterator> entriesRange = entries.equal_range(oneDoc->GetId());
			EntriesMap::iterator entryIt = entriesRange.first;
			while(entryIt != entriesRange.second)
			{
				oneDoc->AddCow(entryIt->second);
				entryIt++;
			}

			result.Add(oneDoc);

			oneDoc = NULL;
			step++;
			if(!loadingSellDocsProgress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneDoc) delete oneDoc;
		BOOST_THROW_EXCEPTION(e);
	}
	progress.EndTask();
	return true;
}

bool MuuDAO::LoadOutDocs(NewPtrFlushArray<OutDoc>& result,uint32_t seasonId,ComplexProgress *pProgress/* = NULL*/)
{
	OutDoc *oneDoc = NULL;
	
	int colDOCID,colDOCDATE,colEXTRAS,colPLATENO,colLOADDATE,colLOADENDTM,colLOADSTRTM,colAGENT,colMOTIVE,colHERD;	

	SQLQuery q;
	Transaction transaction = OpenTransaction();
	
	SafeComplexProgress progress(pProgress);

	SimpleProgress* pLoadingEntriesProgress = progress.BeginTask(L"loading_outdocs_entries");
	std::multimap<uint32_t,CowEntry*> entries;
	
	if(!LoadOutDocCows(entries,seasonId,pLoadingEntriesProgress))
		return false;

	progress.EndTask();
	
	SafeSimpleProgress loadingOutDocsProgress(progress.BeginTask(L"loading_outdocs"));
	int step = 0;
	q << L"SELECT COUNT(*) FROM outdocs WHERE season = %0";
	q.parse();

	int steps = transaction->QueryScalar(q.str(seasonId),0);
	if(steps < 0) return false;
	loadingOutDocsProgress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM outdocs WHERE season = %0";
	q.parse();

	try
	{
		Cursor cursor = transaction->Query(q.str(seasonId));

		colDOCID =		cursor.colIndex(L"DOCID");
		colDOCDATE =	cursor.colIndex(L"DOCDATE");
		colEXTRAS =		cursor.colIndex(L"EXTRAS");		
		colPLATENO =	cursor.colIndex(L"PLATENO");
		colLOADDATE =	cursor.colIndex(L"LOADDATE");
		colLOADENDTM =	cursor.colIndex(L"LOADENDTM");
		colLOADSTRTM =	cursor.colIndex(L"LOADENDTM");
		colAGENT =		cursor.colIndex(L"AGENT");
		colMOTIVE =		cursor.colIndex(L"MOTIVE");
		colHERD =		cursor.colIndex(L"HERD");
				
		while(cursor.nextRow())
		{
			oneDoc = new OutDoc(cursor.getColInt(colDOCID));
							
			oneDoc->SetAgentId(cursor.getColInt(colAGENT,NULL_ID));
			oneDoc->SetDocDate(cursor.getColDateTime(colDOCDATE));
			oneDoc->SetExtras(cursor.getColString(colEXTRAS));
			oneDoc->SetLoadDate(cursor.getColDateTime(colLOADDATE));
			oneDoc->SetLoadEndDtTm(Time(cursor.getColNullInt(colLOADENDTM)));
			oneDoc->SetLoadStartDtTm(Time(cursor.getColNullInt(colLOADSTRTM)));
			oneDoc->SetHerdId(cursor.getColInt(colHERD));		

			std::wstring sMotive = cursor.getColString(colMOTIVE);
			oneDoc->SetMotive((Motive)sMotive[0]);
			oneDoc->SetPlateNo(cursor.getColString(colPLATENO));
			
			std::pair <EntriesMap::iterator, EntriesMap::iterator> entriesRange = entries.equal_range(oneDoc->GetId());
			EntriesMap::iterator entryIt = entriesRange.first;
			while(entryIt != entriesRange.second)
			{
				oneDoc->AddCow(entryIt->second);
				entryIt++;
			}

			result.Add(oneDoc);

			oneDoc = NULL;
			step++;
			if(!loadingOutDocsProgress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneDoc) delete oneDoc;
		BOOST_THROW_EXCEPTION(e);
	}
	progress.EndTask();
	return true;
}


bool MuuDAO::ExtractCowEntries(Cursor& cursor,EntriesMap& result,SimpleProgress *pProgress/* = NULL*/)
{
	CowEntry* oneRow = NULL;
	SafeSimpleProgress progress(pProgress);
	int colCOWID,colDOCID,colGROUP,colSTOCK,colCLASS,colWEIGHT;

	colDOCID =	cursor.colIndex(L"DOC");
	colCOWID =	cursor.colIndex(L"COW");
	colSTOCK =	cursor.colIndex(L"STOCK");		
	colWEIGHT =	cursor.colIndex(L"WEIGHT");
	colCLASS =	cursor.colIndex(L"CLASS");		
	colGROUP =	cursor.colIndex(L"GROUP");
	try
	{
		while(cursor.nextRow())
		{
			oneRow = new CowEntry(cursor.getColInt(colCOWID));
			
			oneRow->SetClassId(cursor.getColInt(colCLASS));
			oneRow->SetGroup(cursor.getColNullInt(colGROUP));
			oneRow->SetStockId(cursor.getColInt(colSTOCK,NULL_ID));
			oneRow->SetWeight(cursor.getColNullDecimal(colWEIGHT));

			result.insert(std::pair<uint32_t,CowEntry*>(cursor.getColInt(colDOCID),oneRow));
					
			if(!progress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;

}

bool MuuDAO::LoadInDocCows(EntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{		
	return LoadCowsFromTable(L"indoc_cows",result,seasonId,pProgress);	
}

bool MuuDAO::LoadBuyDocCows(EntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	return LoadCowsFromTable(L"buydoc_cows",result,seasonId,pProgress);
}

bool MuuDAO::LoadMoveDocCows(EntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	
	return LoadCowsFromTable(L"movedoc_cows",result,seasonId,pProgress);
}

bool MuuDAO::LoadSellDocCows(EntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	return LoadCowsFromTable(L"selldoc_cows",result,seasonId,pProgress);
}

bool MuuDAO::LoadOutDocCows(EntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	return LoadCowsFromTable(L"outdoc_cows",result,seasonId,pProgress);
}

bool MuuDAO::LoadCowsFromTable(const std::wstring& sTableName,EntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	SQLQuery q;
	Transaction transaction = OpenTransaction();
	SafeSimpleProgress progress(pProgress);
		
	q << L"SELECT COUNT(*) FROM %1 WHERE season = %0";
	q.parse();

	int steps = transaction->QueryScalar(q.str(seasonId,sTableName),0);
	if(steps < 0) return false;

	pProgress->SetSteps(steps);


	q.reset();
	q << L"SELECT * FROM %1 WHERE season = %0";
	q.parse();

	Cursor cursor = transaction->Query(q.str(seasonId,sTableName));
	
	return ExtractCowEntries(cursor,result,&progress);
}

bool MuuDAO::LoadBuyInvoices(NewPtrFlushArray<BuyInvoice>& result,uint32_t seasonId,ComplexProgress *pProgress/* = NULL*/)
{
	BuyInvoice *oneRow = NULL;	
	
	int colINVOICEID,colINVOICETYPE,colEXTRAS,colINVOICEDATE,colPAID,colVATRATE,colPAYWAY,
		colCUSTOMNUMBER,colCORRECTFOR,colINVOICENO,colPAYDUEDAYS,colINVOICESCOPE,
		colTRANSACTIONDATE,colTRANSACTIONPLACE;
	
	SQLQuery q;
	Transaction transaction = OpenTransaction();
	
	SafeComplexProgress progress(pProgress);

	SimpleProgress* pLoadingEntriesProgress = progress.BeginTask(L"loading_buyinvoices_entries");
	InvoiceEntriesMap entries;
	
	if(!LoadBuyInvoicesEntries(entries,seasonId,pLoadingEntriesProgress))
		return false;
	progress.EndTask();


	SimpleProgress* pLoadingHentsProgress = progress.BeginTask(L"loading_buyinvoice_hents");

	InvoiceHentsMap hents;
	if(!LoadBuyInvoiceHents(hents,seasonId,pLoadingHentsProgress))
		return false;

	InvoiceDeductionsMap deductions;
	SimpleProgress* pLoadingDeductionsProgress = progress.BeginTask(L"loading_buyinvoice_deductions");
	if (!LoadBuyInvoiceDeductions(deductions, seasonId, pLoadingDeductionsProgress))
		return false;


	 BOOST_SCOPE_EXIT(&hents) 
	 {
        DeepDelete(hents);
	 } 
	 BOOST_SCOPE_EXIT_END


	progress.EndTask();
	
	SafeSimpleProgress loadingBuyInvoicesProgress(progress.BeginTask(L"loading_buyinvoices"));
	int step = 0;
	q << L"SELECT COUNT(*) FROM buy_invoices WHERE season = %0";
	q.parse();

	int steps = transaction->QueryScalar(q.str(seasonId),0);
	if(steps < 0) return false;
	loadingBuyInvoicesProgress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM buy_invoices WHERE season = %0";
	q.parse();

	
	try
	{
		Cursor cursor = transaction->Query(q.str(seasonId));

		colINVOICENO = cursor.colIndex(L"INVOICENO");
		colINVOICEID = cursor.colIndex(L"INVOICEID");
		colINVOICETYPE = cursor.colIndex(L"INVOICE_TYPE");
		colEXTRAS = cursor.colIndex(L"EXTRAS");
		colINVOICEDATE = cursor.colIndex(L"INVOICEDATE");
		colPAID = cursor.colIndex(L"PAID");
		colVATRATE = cursor.colIndex(L"VATRATE");
		colPAYWAY = cursor.colIndex(L"PAYWAY");
		colCUSTOMNUMBER = cursor.colIndex(L"CUSTOMNUMBER");
		colCORRECTFOR = cursor.colIndex(L"CORRECT_FOR");
		colPAYDUEDAYS = cursor.colIndex(L"PAYDUEDAYS");
		colINVOICESCOPE = cursor.colIndex(L"INVOICESCOPE");
		colTRANSACTIONDATE = cursor.colIndex(L"TRANSACTIONDATE");
		colTRANSACTIONPLACE = cursor.colIndex(L"TRANSACTIONPLACE");
		
		while(cursor.nextRow())
		{
			std::wstring sInvoiceScope = cursor.getColString(colINVOICESCOPE);
			InvoiceScope scope = (InvoiceScope)sInvoiceScope[0];

			oneRow = new BuyInvoice(cursor.getColInt(colINVOICEID),scope);
			oneRow->SetInvoiceNo(cursor.getColInt(colINVOICENO));

			std::wstring sInvoiceType = cursor.getColString(colINVOICETYPE);
			oneRow->SetInvoiceType((InvoiceType)sInvoiceType[0]);

			oneRow->SetExtras(cursor.getColString(colEXTRAS)); 
			oneRow->SetInvoiceDate(cursor.getColDateTime(colINVOICEDATE));
			oneRow->SetPaidDate(cursor.getColDateTime(colPAID));
			oneRow->SetVATRate(cursor.getColDecimal(colVATRATE,Decimal(0L)));
						
			std::wstring sPayWay = cursor.getColString(colPAYWAY);
			oneRow->SetPayWay((PayWay)sPayWay[0]);
			oneRow->SetCustomNumber(cursor.getColString(colCUSTOMNUMBER));
			oneRow->SetPayDueDays( cursor.getColInt(colPAYDUEDAYS,0));

			oneRow->SetTransactionDate(cursor.getColDateTime(colTRANSACTIONDATE));
			oneRow->SetTransactionPlace(cursor.getColString(colTRANSACTIONPLACE));		

			std::pair <InvoiceEntriesMap::iterator, InvoiceEntriesMap::iterator> entriesRange = entries.equal_range(oneRow->GetId());
			InvoiceEntriesMap::iterator entryIt = entriesRange.first;
			while(entryIt != entriesRange.second)
			{
				oneRow->AddEntry(entryIt->second);
				entryIt++;
			}

			InvoiceHentsMap::iterator hentIt = hents.find(oneRow->GetId());
			if(hentIt == hents.end())
				BOOST_THROW_EXCEPTION(SQLException(TextUtils::FormatA(L"unable find invoice hent for buy invoice %d", oneRow->GetId())));

			oneRow->SetInvoiceHent(*hentIt->second);

			std::pair <InvoiceDeductionsMap::iterator, InvoiceDeductionsMap::iterator> deductionsRange = deductions.equal_range(oneRow->GetId());
			InvoiceDeductionsMap::iterator deductionsIt = deductionsRange.first;
			while (deductionsIt != deductionsRange.second)
			{
				oneRow->AppendDeduction(deductionsIt->second);
				deductionsIt++;
			}

			result.Add(oneRow);

			oneRow = NULL;
			step++;
			if(!loadingBuyInvoicesProgress.Step()) return false;
			
		}
		
	}
	catch (SQLException& e)
	{
		if(oneRow != NULL) delete oneRow;		
		BOOST_THROW_EXCEPTION(e);
	}
	return true;	
}

bool MuuDAO::LoadSellInvoices(NewPtrFlushArray<SellInvoice>& result,uint32_t seasonId,ComplexProgress *pProgress/* = NULL*/)
{
	SellInvoice *oneRow = NULL;	
	
	int colINVOICEID,colINVOICETYPE,colEXTRAS,colINVOICEDATE,colPAID,colVATRATE,colPAYWAY,
		colCUSTOMNUMBER,colCORRECTFOR,colINVOICENO,colPAYDUEDAYS,colINVOICESCOPE,
		colTRANSACTIONDATE, colTRANSACTIONPLACE;
	
	SQLQuery q;
	Transaction transaction = OpenTransaction();
	
	SafeComplexProgress progress(pProgress);

	SimpleProgress* pLoadingEntriesProgress = progress.BeginTask(L"loading_sellinvoices_entries");
	InvoiceEntriesMap entries;
	
	

	if(!LoadSellInvoicesEntries(entries,seasonId,pLoadingEntriesProgress))
		return false;
	progress.EndTask();

	SimpleProgress* pLoadingHentsProgress = progress.BeginTask(L"loading_sellinvoice_hents");
	InvoiceHentsMap hents;
	if(!LoadSellInvoiceHents(hents,seasonId,pLoadingHentsProgress))
		return false;

	InvoiceDeductionsMap deductions;
	SimpleProgress* pLoadingDeductionsProgress = progress.BeginTask(L"loading_sellinvoice_deductions");
	if (!LoadSellInvoiceDeductions(deductions, seasonId, pLoadingDeductionsProgress))
		return false;


	 BOOST_SCOPE_EXIT(&hents) 
	 {
        DeepDelete(hents);
	 } 
	 BOOST_SCOPE_EXIT_END

	progress.EndTask();
	
	SafeSimpleProgress loadingSellInvoicesProgress(progress.BeginTask(L"loading_sellinvoices"));
	int step = 0;
	q << L"SELECT COUNT(*) FROM sell_invoices WHERE season = %0";
	q.parse();

	int steps = transaction->QueryScalar(q.str(seasonId),0);
	if(steps < 0) return false;
	loadingSellInvoicesProgress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM sell_invoices WHERE season = %0";
	q.parse();
		
	try
	{
		Cursor cursor = transaction->Query(q.str(seasonId));

		colINVOICENO = cursor.colIndex(L"INVOICENO");
		colINVOICEID = cursor.colIndex(L"INVOICEID");
		colINVOICETYPE = cursor.colIndex(L"INVOICE_TYPE");
		colEXTRAS = cursor.colIndex(L"EXTRAS");
		colINVOICEDATE = cursor.colIndex(L"INVOICEDATE");
		colPAID = cursor.colIndex(L"PAID");
		colVATRATE = cursor.colIndex(L"VATRATE");
		colPAYWAY = cursor.colIndex(L"PAYWAY");
		colCUSTOMNUMBER = cursor.colIndex(L"CUSTOMNUMBER");
		colCORRECTFOR = cursor.colIndex(L"CORRECT_FOR");
		colPAYDUEDAYS = cursor.colIndex(L"PAYDUEDAYS");
		colINVOICESCOPE = cursor.colIndex(L"INVOICESCOPE");
		colTRANSACTIONDATE = cursor.colIndex(L"TRANSACTIONDATE");
		colTRANSACTIONPLACE = cursor.colIndex(L"TRANSACTIONPLACE");

		
		while(cursor.nextRow())
		{
			std::wstring sInvoiceScope = cursor.getColString(colINVOICESCOPE);
			InvoiceScope scope = (InvoiceScope)sInvoiceScope[0];

			oneRow = new SellInvoice(cursor.getColInt(colINVOICEID),scope);
			oneRow->SetInvoiceNo(cursor.getColInt(colINVOICENO));

			std::wstring sInvoiceType = cursor.getColString(colINVOICETYPE);
			oneRow->SetInvoiceType((InvoiceType)sInvoiceType[0]);

			oneRow->SetExtras(cursor.getColString(colEXTRAS)); 
			oneRow->SetInvoiceDate(cursor.getColDateTime(colINVOICEDATE));
			oneRow->SetPaidDate(cursor.getColDateTime(colPAID));
			oneRow->SetVATRate(cursor.getColDecimal(colVATRATE,Decimal(0L)));

			
			std::wstring sPayWay = cursor.getColString(colPAYWAY);
			oneRow->SetPayWay((PayWay)sPayWay[0]);
			oneRow->SetCustomNumber(cursor.getColString(colCUSTOMNUMBER));
			oneRow->SetPayDueDays( cursor.getColInt(colPAYDUEDAYS,1));

			oneRow->SetTransactionDate(cursor.getColDateTime(colTRANSACTIONDATE));
			oneRow->SetTransactionPlace(cursor.getColString(colTRANSACTIONPLACE));

			std::pair <InvoiceEntriesMap::iterator, InvoiceEntriesMap::iterator> entriesRange = entries.equal_range(oneRow->GetId());
			InvoiceEntriesMap::iterator entryIt = entriesRange.first;
			while(entryIt != entriesRange.second)
			{
				oneRow->AddEntry(entryIt->second);
				entryIt++;
			}

			InvoiceHentsMap::iterator hentIt = hents.find(oneRow->GetId());
			if(hentIt == hents.end())
				BOOST_THROW_EXCEPTION(SQLException(TextUtils::FormatA(L"unable find invoice hent for sell invoice %d", oneRow->GetId())));

			oneRow->SetInvoiceHent(*hentIt->second);

			std::pair <InvoiceDeductionsMap::iterator, InvoiceDeductionsMap::iterator> deductionsRange = deductions.equal_range(oneRow->GetId());
			InvoiceDeductionsMap::iterator deductionsIt = deductionsRange.first;
			while (deductionsIt != deductionsRange.second)
			{
				oneRow->AppendDeduction(deductionsIt->second);
				deductionsIt++;
			}

			result.Add(oneRow);

			oneRow = NULL;
			step++;
			if(!loadingSellInvoicesProgress.Step()) return false;
			
		}
		
	}
	catch (SQLException& e)
	{
		if(oneRow != NULL) delete oneRow;		
		BOOST_THROW_EXCEPTION(e);
	}
	return true;	

}

bool MuuDAO::LoadBuyInvoicesEntries(InvoiceEntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	return LoadInvoicesEntriesFromTable(L"buy_invoice_entries",result,seasonId,pProgress);
}

bool MuuDAO::LoadSellInvoicesEntries(InvoiceEntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	return LoadInvoicesEntriesFromTable(L"sell_invoice_entries",result,seasonId,pProgress);
}

bool MuuDAO::LoadBuyInvoiceHents(InvoiceHentsMap& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	return LoadInvoiceHentsFromTable(L"buy_invoice_hents",result,seasonId,pProgress);
}

bool MuuDAO::LoadSellInvoiceHents(InvoiceHentsMap& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	return LoadInvoiceHentsFromTable(L"sell_invoice_hents",result,seasonId,pProgress);
}


bool MuuDAO::LoadInvoiceHentsFromTable(const std::wstring& sTableName,InvoiceHentsMap& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	SQLQuery q;
	Transaction transaction = OpenTransaction();
	SafeSimpleProgress progress(pProgress);
		
	q << L"SELECT COUNT(*) FROM %1 WHERE season = %0";
	q.parse();

	int steps = transaction->QueryScalar(q.str(seasonId,sTableName),0);
	if(steps < 0) return false;

	progress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM %1 WHERE season = %0";
	q.parse();

	Cursor cursor = transaction->Query(q.str(seasonId,sTableName));
	
	return ExtractInvoiceHents(cursor,result,&progress);
}

bool MuuDAO::ExtractInvoiceHents(Cursor& cursor,InvoiceHentsMap& result,SimpleProgress *pProgress/* = NULL*/)
{
	InvoiceHent *oneHent = NULL;

	int colHENTNO,colCITY,colINVOICE,colNAME,colNIP,colPOBOX,colSTREET,colZIP,colPESEL,colIDNO,
		colISSUEDATE,colISSUEPOST,colACCOUNTNO,colBANKNAME,colPARENTHENT;
		
	try
	{	
		colHENTNO = cursor.colIndex(L"hentno");	   
		colCITY = cursor.colIndex(L"city");
		colINVOICE = cursor.colIndex(L"invoice");		
		colNAME = cursor.colIndex(L"name");
		colNIP = cursor.colIndex(L"fiscal_no");		
		colPOBOX = cursor.colIndex(L"pobox");
		colSTREET = cursor.colIndex(L"street");		
		colZIP = cursor.colIndex(L"zip");		
		colPESEL = cursor.colIndex(L"personal_no");		
		colIDNO = cursor.colIndex(L"personal_id_no");
		colISSUEDATE = cursor.colIndex(L"issuedate");
		colISSUEPOST = cursor.colIndex(L"issuepost");
		colACCOUNTNO = cursor.colIndex(L"accountno");
		colBANKNAME = cursor.colIndex(L"bankname");	
		colPARENTHENT = cursor.colIndex(L"parent_hent");
        		
		while(cursor.nextRow())
		{			
			oneHent = new InvoiceHent();
						
			oneHent->SetHentNo(HentNoFactory::FromString(cursor.getColString(colHENTNO)));			
			oneHent->SetCity(cursor.getColString(colCITY));			
			oneHent->SetName(cursor.getColString(colNAME));
			oneHent->SetFiscalNo(cursor.getColString(colNIP));			
			oneHent->SetPOBox(cursor.getColString(colPOBOX));
			oneHent->SetStreet(cursor.getColString(colSTREET));		
			oneHent->SetZip(cursor.getColString(colZIP));			
			oneHent->SetPESEL(cursor.getColString(colPESEL));			
			oneHent->SetIdNo(cursor.getColString(colIDNO));
			oneHent->SetIssueDate(cursor.getColDateTime(colISSUEDATE));
			oneHent->SetIssuePost(cursor.getColString(colISSUEPOST));
			oneHent->SetAccountNo(AccountNoFactory::FromString(cursor.getColString(colACCOUNTNO)));
			oneHent->SetBankName(cursor.getColString(colBANKNAME));
			oneHent->SetParentHentId(cursor.getColInt(colPARENTHENT));

			result[cursor.getColInt(colINVOICE)] = oneHent;
			oneHent = NULL;			
			if(!pProgress->Step()) return false;
	    }	
	}
	catch (SQLException e)
	{
		if(oneHent) delete oneHent;
		BOOST_THROW_EXCEPTION(e);
	}

	return true;

}


bool MuuDAO::LoadInvoicesEntriesFromTable(const std::wstring& sTableName,InvoiceEntriesMap& result,uint32_t seasonId,SimpleProgress *pProgress/* = NULL*/)
{
	SQLQuery q;
	Transaction transaction = OpenTransaction();
	SafeSimpleProgress progress(pProgress);
		
	q << L"SELECT COUNT(*) FROM %1 WHERE season = %0";
	q.parse();

	int steps = transaction->QueryScalar(q.str(seasonId,sTableName),0);
	if(steps < 0) return false;

	progress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM %1 WHERE season = %0";
	q.parse();

	Cursor cursor = transaction->Query(q.str(seasonId,sTableName));
	
	return ExtractInvoiceEntries(cursor,result,&progress);
}

bool MuuDAO::ExtractInvoiceEntries(Cursor& cursor,InvoiceEntriesMap& result,SimpleProgress *pProgress/* = NULL*/)
{
	CowInvoiceEntry* oneRow = NULL;
	SafeSimpleProgress progress(pProgress);
	int colCOW,colINVOICE,colCLASS,colWEIGHT,colPRICE;

	colINVOICE = cursor.colIndex(L"INVOICE");
	colCOW = cursor.colIndex(L"COW");
	colPRICE = cursor.colIndex(L"PRICE");		
	colWEIGHT =	cursor.colIndex(L"WEIGHT");
	colCLASS = cursor.colIndex(L"CLASS");		
	
	try
	{
		while(cursor.nextRow())
		{
			oneRow = new CowInvoiceEntry(cursor.getColInt(colCOW));
			
			oneRow->SetClassId(cursor.getColInt(colCLASS));
			oneRow->SetPrice(cursor.getColDecimal(colPRICE,Decimal(0L)));
			oneRow->SetWeight(cursor.getColDecimal(colWEIGHT,Decimal(0L)));

			result.insert(std::pair<uint32_t,CowInvoiceEntry*>(cursor.getColInt(colINVOICE),oneRow));
					
			if(!progress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}

bool MuuDAO::LoadAgents(NewPtrFlushArray<Agent>& result, uint32_t farmId, SimpleProgress *pProgress/* = NULL*/)
{
	Agent *oneRow = NULL;
	
	int colNAME,colCODE,colID,colPLATE;

	SQLQuery q;
	q << L"SELECT COUNT(*) FROM agents WHERE farm = %0";
	q.parse();

	Transaction transaction = OpenTransaction();

	int step = 0;
	int steps = transaction->QueryScalar(q.str(farmId), 0);
	if(steps < 0) return false;

	SafeSimpleProgress progress(pProgress);

	progress.SetSteps(steps);

	result.RemoveAll();

	try
	{		
		q.reset();
		q << L"SELECT * FROM agents WHERE farm = %0";
		q.parse();
		Cursor cursor = transaction->Query(q.str(farmId));

		colNAME = cursor.colIndex(L"agentname");
		colCODE  = cursor.colIndex(L"agentcode");
		colID = cursor.colIndex(L"agentid");	
		colPLATE = cursor.colIndex(L"plate");
		
		while(cursor.nextRow())
		{	
		    oneRow =  new Agent(cursor.getColInt(colID));
				
			oneRow->SetAgentName(cursor.getColString(colNAME));
			oneRow->SetAgentCode(cursor.getColString(colCODE));			
			oneRow->SetPlateNo(cursor.getColString(colPLATE));
			
			result.Add(oneRow);
			oneRow = NULL;
			if(!progress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}

bool MuuDAO::UpdateAgent(Transaction transaction,IAgent* pHent)
{
	return false;
}

bool MuuDAO::DeleteAgent(Transaction transaction,IAgent* pHent)
{
	return false;
}

Agent* MuuDAO::InsertAgent(Transaction transaction,uint32_t season,IAgent* pAgent)
{
	return NULL;
}


Cow* MuuDAO::InsertCow(Transaction transaction,uint32_t season,const CowDetails& cowDetails)
{	
	SQLQuery q;
	q <<  L"INSERT INTO cattle("
		  L"cowno,"					//0
		  L"birthdate,"				//1
		  L"birthplace,"			//2
		  L"sex,"					//3
		  L"stock,"					//4
		  L"motherno,"				//5
		  L"weight,"				//6
		  L"passno,"				//7
		  L"passdate,"				//8
		  L"fstownr,"				//9		
		  L"extras,"				//10			 
		  L"healthcertno,"			//11
		  L"class,"					//12
		  L"season,"				//13
		  L"animalid"				//14	
		  L") VALUES("
		  L"%0n,"
		  L"%1n,"
		  L"%2n,"
		  L"%3,"
		  L"%4,"
		  L"%5n,"
		  L"%6,"
		  L"%7n,"
		  L"%8n,"
		  L"%9,"
		  L"%10n,"		  
		  L"%11n,"
		  L"%12,"
		  L"%13,"
		  L"%14"
		  L");";

	q.parse();

	uint32_t cowId = NextSeqValue(transaction,L"cattle_seq");

	SQLQueryParms params;
	params	<< cowDetails.GetCowNo()				//0
			<< cowDetails.GetBirthDate()			//1
			<< cowDetails.GetBirthPlace()			//2
			<< cowDetails.GetSex()					//3
			<< SQLIdable(cowDetails.GetStockId())	//4
			<< cowDetails.GetMotherNo()				//5
			<< cowDetails.GetWeight()				//6
			<< cowDetails.GetPassNo()				//7
			<< SQLString(cowDetails.GetPassDate(),true)	//8
			<< SQLIdable(cowDetails.GetFirstOwnerId())	//9
			<< cowDetails.GetExtras()					//10
			<< cowDetails.GetHealthCertNo()				//11
			<< cowDetails.GetClassId()					//12
			<< season									//13
			<< cowId									//14
			;
		  
	transaction->Insert(q.str(params).c_str());

	Cow* pCow = new Cow(cowId);
	pCow->SetFrom(cowDetails);
	return pCow;
}

bool MuuDAO::DeleteCow(Transaction transaction,uint32_t cowId)
{
	SQLQuery q;
	q << L"DELETE FROM cattle where animalid = " << cowId;
	return transaction->Delete(q.str().c_str());
}

void MuuDAO::UpdateCow(Transaction transaction,uint32_t cowId,const CowDetails& cowDetails)
{
	SQLQuery q;
	q <<  "UPDATE cattle SET "
		  "cowno = %0n,"
		  "birthdate = %1n,"
		  "birthplace = %2n,"
		  "sex = %3,"
		  "stock = %4,"
		  "motherno = %5n,"
		  "weight = %6,"
		  "passno = %7n,"
		  "passdate = %8n,"
		  "fstownr = %9,"
		  "extras = %10n,"		  
		  "healthcertno = %11n,"
		  "class = %12"		 
		  " WHERE animalid = %13";

	q.parse();

	SQLQueryParms params;

	params	<< cowDetails.GetCowNo()						//0
			<< SQLString(cowDetails.GetBirthDate(),true)	//1
			<< cowDetails.GetBirthPlace()					//2
			<< cowDetails.GetSex()							//3
			<< SQLIdable(cowDetails.GetStockId())			//4
			<< cowDetails.GetMotherNo()						//5
			<< cowDetails.GetWeight()						//6
			<< cowDetails.GetPassNo()						//7
			<< SQLString(cowDetails.GetPassDate(),true)		//8
			<< SQLIdable(cowDetails.GetFirstOwnerId())		//9
			<< cowDetails.GetExtras()						//10
			<< cowDetails.GetHealthCertNo()					//11
			<< cowDetails.GetClassId()						//12
			<< cowId;										//13

	transaction->Update(q.str(params).c_str());
}

void MuuDAO::UpdateCow(Transaction transaction,ICow* pCow)
{
	SQLQuery q;
	q <<  L"UPDATE cattle SET"
		  L"cowno = %0n,"
		  L"birthdate = %1n,"
		  L"birthplace = %2n,"
		  L"sex = %3,"
		  L"stock = %4,"
		  L"motherno = %5n,"
		  L"weight = %6,"
		  L"passno = %7n,"
		  L"passdate = %8n,"
		  L"fstownr = %9,"
		  L"extras = %10,"		  
		  L"healthcertno = %11n,"
		  L"class = %12,"
		  L"termbuyclass = %13"
		  L"termbuyweight = %14"
		  L"termsellclass = %15"
		  L"termsellweight = %16"
		  L"termbuyprice = %17"
		  L"termsellprice = %18"
		  L"passimglabel = %19"
		  L"sell_invoice = %20"
		  L"buy_invoice = %21"
		  L" WHERE animalid = %22";

	q.parse();

	SQLQueryParms params;

	params	<< pCow->GetCowNo()						//0
			<< SQLString(pCow->GetBirthDate(),true)//1
			<< pCow->GetBirthPlace()				//2
			<< pCow->GetSex()						//3
			<< SQLIdable(pCow->GetStockId())		//4
			<< pCow->GetMotherNo()					//5
			<< pCow->GetWeight()					//6
			<< pCow->GetPassNo()					//7
			<< SQLString(pCow->GetPassDate(),true)  //8
			<< SQLIdable(pCow->GetFirstOwnerId())	//9
			<< pCow->GetExtras()					//10
			<< pCow->GetHealthCertNo()				//11
			<< pCow->GetClassId()					//12
			<< SQLIdable(pCow->GetTermBuyClassId())	//13
			<< pCow->GetTermBuyWeight()				//14
			<< SQLIdable(pCow->GetTermSellClassId())//15
			<< pCow->GetTermSellWeight()			//16
			<< pCow->GetTermBuyPrice()				//17
			<< pCow->GetTermSellPrice()				//18
			<< pCow->GetPassImgLabel()				//19
			<< SQLIdable(pCow->GetSellInvoiceId())	//20
			<< SQLIdable(pCow->GetBuyInvoiceId())	//21
			<< pCow->GetId();						//22

	transaction->Update(q.str(params).c_str());	
}

InDoc* MuuDAO::InsertInDoc(Transaction transaction,uint32_t season,IInDoc* pDoc)
{
	SQLQuery q;
	q << L"INSERT INTO indocs("
		 L"docdate,"		//0
		 L"extras,"		//1
		 L"plateno,"		//2
		 L"loaddate,"	//3
		 L"motive,"		//4
		 L"loadstrtm,"	//5
		 L"loadendtm,"	//6
		 L"agent,"		//7
		 L"season,"		//8
		 L"herd,"		//9
		 L"docid"		//10
		 L") VALUES("
		 L"%0,"
		 L"%1n,"
		 L"%2n,"
		 L"%3,"
		 L"%4q,"
		 L"%5,"
		 L"%6,"
		 L"%7,"		 
		 L"%8,"
		 L"%9,"
		 L"%10"
		 L");";
	

	q.parse();
	
	uint32_t docId = NextSeqValue(transaction,L"indocs_seq");

	SQLQueryParms params;

	params  << SQLString(pDoc->GetDocDate(),true)	//0
			<< pDoc->GetExtras()					//1
			<< pDoc->GetPlateNo()					//2
			<< SQLString(pDoc->GetLoadDate(),true)	//3
			<< pDoc->GetMotive()					//4
			<< pDoc->GetLoadStartDtTm()				//5
			<< pDoc->GetLoadEndDtTm()				//6
			<< SQLIdable(pDoc->GetAgentId())		//7
			<< season								//8
			<< pDoc->GetHerdId()					//9
			<< docId								//10
			;

	InDoc* pNewInDoc = NULL;
	CowEntry* pNewEntry = NULL;
	try
	{
		transaction->Insert(q.str(params).c_str());
		
		pNewInDoc = new InDoc(docId);
		pNewInDoc->CopyFrom(pDoc);

		PtrEnumerator<ICowEntry> entryEnum = pNewInDoc->EnumCows();
		ICowEntry* pEntry;
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			InsertInDocCowEntry(transaction,season,docId,pEntry);
			entryEnum.advance();
		}

	}
	catch(...)
	{
		if(pNewInDoc)
			delete pNewInDoc;
		std::rethrow_exception(std::current_exception());
	}
	
	return pNewInDoc;
}


bool MuuDAO::DeleteInDoc(Transaction transaction,uint32_t docId)
{	
	if(DeleteInDocCowEntries(transaction,docId))
	{
		SQLQuery q;
		q << L"DELETE FROM indocs where docid = %0";
		q.parse();
		return transaction->Delete(q.str(docId).c_str());
	}
	return false;
}

void MuuDAO::UpdateInDoc(Transaction transaction,const IInDoc* pDoc)
{
	SQLQuery seasonQ;
	seasonQ << L"SELECT season from indocs WHERE docid = " << pDoc->GetId();
	
	uint32_t seasonId = transaction->QueryScalar(seasonQ.str(),NULL_ID);

	SQLQuery stmt;
	stmt << "UPDATE indocs SET "
		 "docdate = %0,"		//0
		 "extras = %1n,"		//1
		 "plateno = %2n,"		//2
		 "loaddate = %3,"		//3
		 "motive = %4q,"		//4
		 "loadstrtm = %5,"		//5
		 "loadendtm = %6,"		//6
		 "agent = %7"			//7
		 " WHERE docid = %8;";
	
	stmt.parse();

	SQLQueryParms params;

	params  << SQLString(pDoc->GetDocDate(),true)	//0
			<< pDoc->GetExtras()					//1
			<< pDoc->GetPlateNo()					//2
			<< SQLString(pDoc->GetLoadDate(),true)	//3
			<< pDoc->GetMotive()					//4
			<< pDoc->GetLoadStartDtTm()				//5
			<< pDoc->GetLoadEndDtTm()				//6
			<< SQLIdable(pDoc->GetAgentId())		//7
			<< pDoc->GetId()						//8
			;

	transaction->Update(stmt.str(params).c_str());
	
	DeleteInDocCowEntries(transaction,pDoc->GetId());

	PtrEnumerator<ICowEntry> entryEnum = pDoc->EnumCows();
	ICowEntry* pEntry;
	while(entryEnum.hasNext())
	{
		pEntry = *entryEnum;
		InsertInDocCowEntry(transaction,seasonId,pDoc->GetId(),pEntry);
		entryEnum.advance();
	}	
}

void MuuDAO::InsertCowEntry(Transaction transaction,const std::wstring& sTable,uint32_t season,uint32_t docId,const ICowEntry* pEntry)
{
	SQLQuery q;
	q << L"INSERT INTO %0 (cow, doc, \"group\", stock, weight, season, class) VALUES ("
		 L"%1," 
		 L"%2," 
		 L"%3," 
		 L"%4," 
		 L"%5," 
		 L"%6,"
		 L"%7"
		 L");";

	q.parse();

	SQLQueryParms params;

	params	<< sTable
			<< pEntry->GetCowId()
			<< docId
			<< pEntry->GetGroup()
			<< SQLIdable(pEntry->GetStockId())
			<< pEntry->GetWeight()
			<< season
			<< SQLIdable(pEntry->GetClassId());

	transaction->Insert(q.str(params).c_str());
}

void MuuDAO::InsertInDocCowEntry(Transaction transaction,uint32_t season,uint32_t docId,const ICowEntry* pEntry)
{
	InsertCowEntry(transaction,L"indoc_cows",season,docId,pEntry);
}

BuyDoc* MuuDAO::InsertBuyDoc(Transaction transaction,uint32_t season,IBuyDoc* pDoc)
{
	SQLQuery q;
	q << L"INSERT INTO buydocs("
		 L"docdate,"		//0
		 L"extras,"		//1
		 L"plateno,"		//2
		 L"loaddate,"	//3
		 L"motive,"		//4
		 L"loadstrtm,"	//5
		 L"loadendtm,"	//6
		 L"agent,"		//7
		 L"season,"		//8
		 L"herd,"		//9
		 L"hent,"		//10
		 L"docid"		//11
		 L") VALUES("
		 L"%0,"
		 L"%1q,"
		 L"%2q,"
		 L"%3,"
		 L"%4q,"
		 L"%5,"
		 L"%6,"
		 L"%7,"		 
		 L"%8,"
		 L"%9,"
		 L"%10,"
		 L"%11"
		 L");";
	

	q.parse();

	uint32_t docId = NextSeqValue(transaction,L"buydocs_seq");
	
	SQLQueryParms params;

	params  << SQLString(pDoc->GetDocDate(),true)	//0
			<< pDoc->GetExtras()					//1
			<< pDoc->GetPlateNo()					//2
			<< SQLString(pDoc->GetLoadDate(),true)	//3
			<< pDoc->GetMotive()					//4
			<< pDoc->GetLoadStartDtTm()				//5
			<< pDoc->GetLoadEndDtTm()				//6
			<< SQLIdable(pDoc->GetAgentId())		//7
			<< season								//8
			<< pDoc->GetHerdId()					//9
			<< pDoc->GetHentId()					//10
			<< docId								//11
			;

	BuyDoc* pNewDoc = NULL;
	CowEntry* pNewEntry = NULL;
	try
	{
		transaction->Insert(q.str(params).c_str());
		
		pNewDoc = new BuyDoc(docId);
		pNewDoc->CopyFrom(pDoc);

		PtrEnumerator<ICowEntry> entryEnum = pNewDoc->EnumCows();
		ICowEntry* pEntry;
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			InsertBuyDocCowEntry(transaction,season,docId,pEntry);
			entryEnum.advance();
		}

	}
	catch(...)
	{
		if(pNewDoc)
			delete pNewDoc;
		std::rethrow_exception(std::current_exception());
	}
	
	return pNewDoc;
}

void MuuDAO::InsertBuyDocCowEntry(Transaction transaction,uint32_t season,uint32_t docId,const ICowEntry* pEntry)
{
	InsertCowEntry(transaction,L"buydoc_cows",season,docId,pEntry);
}

void MuuDAO::UpdateBuyDoc(Transaction transaction,const IBuyDoc* pDoc)
{
	SQLQuery seasonQ;
	seasonQ << L"SELECT season from buydocs WHERE docid = " << pDoc->GetId();
	
	uint32_t seasonId = transaction->QueryScalar(seasonQ.str(),NULL_ID);

	SQLQuery stmt;
	stmt << L"UPDATE buydocs SET "
			L"docdate = %0,"			//0
			L"extras = %1n,"			//1
			L"plateno = %2n,"		//2
			L"loaddate = %3,"		//3
			L"motive = %4q,"			//4
			L"loadstrtm = %5,"		//5
			L"loadendtm = %6,"		//6
			L"agent = %7,"			//7
			L"hent = %8"				//8
			L" WHERE docid = %9;";
	
	stmt.parse();

	SQLQueryParms params;

	params  << SQLString(pDoc->GetDocDate(),true)	//0
			<< pDoc->GetExtras()					//1
			<< pDoc->GetPlateNo()					//2
			<< SQLString(pDoc->GetLoadDate(),true)	//3
			<< pDoc->GetMotive()					//4
			<< pDoc->GetLoadStartDtTm()				//5
			<< pDoc->GetLoadEndDtTm()				//6
			<< SQLIdable(pDoc->GetAgentId())		//7
			<< pDoc->GetHentId()					//8
			<< pDoc->GetId()						//9
			;

	transaction->Update(stmt.str(params).c_str());
	
	DeleteBuyDocCowEntries(transaction,pDoc->GetId());

	PtrEnumerator<ICowEntry> entryEnum = pDoc->EnumCows();
	ICowEntry* pEntry;
	while(entryEnum.hasNext())
	{
		pEntry = *entryEnum;
		InsertBuyDocCowEntry(transaction,seasonId,pDoc->GetId(),pEntry);
		entryEnum.advance();
	}	

}

bool MuuDAO::DeleteBuyDoc(Transaction transaction,uint32_t docId)
{
	if(DeleteBuyDocCowEntries(transaction,docId))
	{
		SQLQuery q;
		q << L"DELETE FROM buydocs where docid = %0";
		q.parse();
		return transaction->Delete(q.str(docId).c_str());
	}
	return false;
}

void MuuDAO::InsertMoveDocCowEntry(Transaction transaction,uint32_t season,uint32_t docId,const ICowEntry* pEntry)
{
	InsertCowEntry(transaction,L"movedoc_cows",season,docId,pEntry);
}

MoveDoc* MuuDAO::InsertMoveDoc(Transaction transaction,uint32_t season,IMoveDoc* pDoc)
{
	SQLQuery q;
	q << L"INSERT INTO movedocs("
		 L"docdate,"		//0
		 L"extras,"		//1
		 L"plateno,"		//2
		 L"loaddate,"	//3
		 L"loadstrtm,"	//4
		 L"loadendtm,"	//5
		 L"agent,"		//6
		 L"season,"		//7
		 L"herd_from,"	//8
		 L"herd_to,"		//9
		 L"docid"		//10
		 L") VALUES("
		 L"%0,"
		 L"%1n,"
		 L"%2n,"
		 L"%3,"		 
		 L"%4,"
		 L"%5,"
		 L"%6,"		 
		 L"%7,"
		 L"%8,"
		 L"%9,"
		 L"%10"
		 L");";
	

	q.parse();

	uint32_t docId = NextSeqValue(transaction,L"movedocs_seq");
	
	SQLQueryParms params;

	params  << SQLString(pDoc->GetDocDate(),true)	//0
			<< pDoc->GetExtras()					//1
			<< pDoc->GetPlateNo()					//2
			<< SQLString(pDoc->GetLoadDate(),true)	//3			
			<< pDoc->GetLoadStartDtTm()				//4
			<< pDoc->GetLoadEndDtTm()				//5
			<< SQLIdable(pDoc->GetAgentId())		//6
			<< season								//7
			<< pDoc->GetSrcHerdId()					//8
			<< pDoc->GetDstHerdId()					//9
			<< docId								//10
			;

	MoveDoc* pNewDoc = NULL;
	CowEntry* pNewEntry = NULL;
	try
	{
		transaction->Insert(q.str(params).c_str());
		
		pNewDoc = new MoveDoc(docId);
		pNewDoc->CopyFrom(pDoc);
		PtrEnumerator<ICowEntry> entryEnum = pNewDoc->EnumCows();
		ICowEntry* pEntry;
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			InsertMoveDocCowEntry(transaction,season,docId,pEntry);
			entryEnum.advance();
		}

	}
	catch(...)
	{
		if(pNewDoc)
			delete pNewDoc;
		std::rethrow_exception(std::current_exception());
	}
	
	return pNewDoc;

}

bool MuuDAO::DeleteMoveDoc(Transaction transaction,uint32_t docId)
{
	if(DeleteMoveDocCowEntries(transaction,docId))
	{
		SQLQuery q;
		q << L"DELETE FROM movedocs where docid = %0";
		q.parse();
		return transaction->Delete(q.str(docId).c_str());
	}
	return false;
}

void MuuDAO::UpdateMoveDoc(Transaction transaction,const IMoveDoc* pDoc)
{
	SQLQuery seasonQ;
	seasonQ << L"SELECT season from movedocs WHERE docid = " << pDoc->GetId();
	
	uint32_t seasonId = transaction->QueryScalar(seasonQ.str(),NULL_ID);

	SQLQuery stmt;
	stmt << L"UPDATE movedocs SET "
			L"docdate = %0,"		//0
			L"extras = %1n,"		//1
			L"plateno = %2n,"		//2
			L"loaddate = %3,"		//3
			L"loadstrtm = %4,"		//4
			L"loadendtm = %5,"		//5
			L"agent = %6"			//6
			L" WHERE docid = %7;";
	
	stmt.parse();

	SQLQueryParms params;

	params  << SQLString(pDoc->GetDocDate(),true)	//0
			<< pDoc->GetExtras()					//1
			<< pDoc->GetPlateNo()					//2
			<< SQLString(pDoc->GetLoadDate(),true)	//3
			<< pDoc->GetLoadStartDtTm()				//4
			<< pDoc->GetLoadEndDtTm()				//5
			<< SQLIdable(pDoc->GetAgentId())		//6
			<< pDoc->GetId()						//7
			;

	transaction->Update(stmt.str(params).c_str());
	
	DeleteMoveDocCowEntries(transaction,pDoc->GetId());

	PtrEnumerator<ICowEntry> entryEnum = pDoc->EnumCows();
	ICowEntry* pEntry;
	while(entryEnum.hasNext())
	{
		pEntry = *entryEnum;
		InsertMoveDocCowEntry(transaction,seasonId,pDoc->GetId(),pEntry);
		entryEnum.advance();
	}	
}


SellDoc* MuuDAO::InsertSellDoc(Transaction transaction,uint32_t season,ISellDoc* pDoc)
{
	SQLQuery q;
	q << L"INSERT INTO selldocs("
		 L"docdate,"		//0
		 L"extras,"		//1
		 L"plateno,"		//2
		 L"loaddate,"	//3
		 L"motive,"		//4
		 L"loadstrtm,"	//5
		 L"loadendtm,"	//6
		 L"agent,"		//7
		 L"season,"		//8
		 L"herd,"		//9
		 L"hent,"		//10
		 L"docid"		//11
		 L") VALUES("
		 L"%0,"
		 L"%1n,"
		 L"%2n,"
		 L"%3,"
		 L"%4q,"
		 L"%5,"
		 L"%6,"
		 L"%7,"		 
		 L"%8,"
		 L"%9,"
		 L"%10,"
		 L"%11"
		 L");";
	
	q.parse();

	uint32_t docId = NextSeqValue(transaction,L"selldocs_seq");
	
	SQLQueryParms params;

	params  << SQLString(pDoc->GetDocDate(),true)	//0
			<< pDoc->GetExtras()					//1
			<< pDoc->GetPlateNo()					//2
			<< SQLString(pDoc->GetLoadDate(),true)	//3
			<<pDoc->GetMotive()						//4
			<< pDoc->GetLoadStartDtTm()				//5
			<< pDoc->GetLoadEndDtTm()				//6
			<< SQLIdable(pDoc->GetAgentId())		//7
			<< season								//8
			<< pDoc->GetHerdId()					//9
			<< pDoc->GetHentId()					//10
			<< docId								//11
			;

	SellDoc* pNewDoc = NULL;
	CowEntry* pNewEntry = NULL;
	try
	{
		transaction->Insert(q.str(params).c_str());
		
		pNewDoc = new SellDoc(docId);
		pNewDoc->CopyFrom(pDoc);
		PtrEnumerator<ICowEntry> entryEnum = pNewDoc->EnumCows();
		ICowEntry* pEntry;
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			InsertSellDocCowEntry(transaction,season,docId,pEntry);
			entryEnum.advance();
		}

	}
	catch(...)
	{
		if(pNewDoc)
			delete pNewDoc;
		std::rethrow_exception(std::current_exception());
	}
	
	return pNewDoc;

}

void MuuDAO::InsertSellDocCowEntry(Transaction transaction,uint32_t season,uint32_t docId,const ICowEntry* pEntry)
{
	InsertCowEntry(transaction,L"selldoc_cows",season,docId,pEntry);
}

void MuuDAO::UpdateSellDoc(Transaction transaction,ISellDoc* pDoc)
{
	SQLQuery seasonQ;
	seasonQ << L"SELECT season from selldocs WHERE docid = " << pDoc->GetId();
	
	uint32_t seasonId = transaction->QueryScalar(seasonQ.str(),NULL_ID);

	SQLQuery stmt;
	stmt << L"UPDATE selldocs SET "
			L"docdate = %0,"			//0
			L"extras = %1n,"			//1
			L"plateno = %2n,"		//2
			L"loaddate = %3,"		//3
			L"motive = %4q,"			//4
			L"loadstrtm = %5,"		//5
			L"loadendtm = %6,"		//6
			L"agent = %7,"			//7
			L"hent = %8"				//8
			L" WHERE docid = %9;";
	
	stmt.parse();

	SQLQueryParms params;

	params  << SQLString(pDoc->GetDocDate(),true)	//0
			<< pDoc->GetExtras()					//1
			<< pDoc->GetPlateNo()					//2
			<< SQLString(pDoc->GetLoadDate(),true)	//3
			<< pDoc->GetMotive()					//4
			<< pDoc->GetLoadStartDtTm()				//5
			<< pDoc->GetLoadEndDtTm()				//6
			<< SQLIdable(pDoc->GetAgentId())		//7
			<< pDoc->GetHentId()					//8
			<< pDoc->GetId()						//9
			;

	transaction->Update(stmt.str(params).c_str());
	
	DeleteSellDocCowEntries(transaction,pDoc->GetId());

	PtrEnumerator<ICowEntry> entryEnum = pDoc->EnumCows();
	ICowEntry* pEntry;
	while(entryEnum.hasNext())
	{
		pEntry = *entryEnum;
		InsertSellDocCowEntry(transaction,seasonId,pDoc->GetId(),pEntry);
		entryEnum.advance();
	}
}


bool MuuDAO::DeleteSellDoc(Transaction transaction,uint32_t docId)
{
	if(DeleteSellDocCowEntries(transaction,docId))
	{
		SQLQuery q;
		q << L"DELETE FROM selldocs where docid = %0";
		q.parse();
		return transaction->Delete(q.str(docId).c_str());
	}
	return false;
}

OutDoc* MuuDAO::InsertOutDoc(Transaction transaction,uint32_t season,IOutDoc* pDoc)
{
	SQLQuery q;
	q << L"INSERT INTO outdocs("
		 L"docdate,"		//0
		 L"extras,"		//1
		 L"plateno,"		//2
		 L"loaddate,"	//3
		 L"motive,"		//4
		 L"loadstrtm,"	//5
		 L"loadendtm,"	//6
		 L"agent,"		//7
		 L"season,"		//8
		 L"herd,"		//9		 
		 L"docid"		//10
		 L") VALUES("
		 L"%0,"
		 L"%1n,"
		 L"%2n,"
		 L"%3,"
		 L"%4q,"
		 L"%5,"
		 L"%6,"
		 L"%7,"		 
		 L"%8,"
		 L"%9,"
		 L"%10"
		 L");";
	

	q.parse();

	uint32_t docId = NextSeqValue(transaction,L"outdocs_seq");
	
	SQLQueryParms params;

	params  << SQLString(pDoc->GetDocDate(),true)	//0
			<< pDoc->GetExtras()					//1
			<< pDoc->GetPlateNo()					//2
			<< SQLString(pDoc->GetLoadDate(),true)	//3
			<< pDoc->GetMotive()					//4
			<< pDoc->GetLoadStartDtTm()				//5
			<< pDoc->GetLoadEndDtTm()				//6
			<< SQLIdable(pDoc->GetAgentId())		//7
			<< season								//8
			<< pDoc->GetHerdId()					//9		
			<< docId								//10
			;

	OutDoc* pNewDoc = NULL;
	CowEntry* pNewEntry = NULL;
	try
	{
		transaction->Insert(q.str(params).c_str());
		
		pNewDoc = new OutDoc(docId);
		pNewDoc->CopyFrom(pDoc);
		PtrEnumerator<ICowEntry> entryEnum = pNewDoc->EnumCows();
		ICowEntry* pEntry;
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			InsertOutDocCowEntry(transaction,season,docId,pEntry);
			entryEnum.advance();
		}

	}
	catch(...)
	{
		if(pNewDoc)
			delete pNewDoc;
		std::rethrow_exception(std::current_exception());
	}
	
	return pNewDoc;
}

void MuuDAO::UpdateOutDoc(Transaction transaction,IOutDoc* pDoc)
{
	SQLQuery seasonQ;
	seasonQ << L"SELECT season from outdocs WHERE docid = " << pDoc->GetId();
	
	uint32_t seasonId = transaction->QueryScalar(seasonQ.str(),NULL_ID);

	SQLQuery stmt;
	stmt << L"UPDATE outdocs SET "
			L"docdate = %1,"			//1
			L"extras = %2n,"			//2
			L"plateno = %3n,"		//3
			L"loaddate = %4,"		//4
			L"motive = %5q,"			//5
			L"loadstrtm = %6,"		//6
			L"loadendtm = %7,"		//7
			L"agent = %8"			//8
			L" WHERE docid = %0;";
	
	stmt.parse();

	SQLQueryParms params;

	params  << pDoc->GetId()						//0
			<< SQLString(pDoc->GetDocDate(),true)	//1
			<< pDoc->GetExtras()					//2
			<< pDoc->GetPlateNo()					//3
			<< SQLString(pDoc->GetLoadDate(),true)	//4
			<< pDoc->GetMotive()					//5
			<< pDoc->GetLoadStartDtTm()				//6
			<< pDoc->GetLoadEndDtTm()				//7
			<< SQLIdable(pDoc->GetAgentId())		//8
			;

	transaction->Update(stmt.str(params).c_str());
	
	DeleteOutDocCowEntries(transaction,pDoc->GetId());

	PtrEnumerator<ICowEntry> entryEnum = pDoc->EnumCows();
	ICowEntry* pEntry;
	while(entryEnum.hasNext())
	{
		pEntry = *entryEnum;
		InsertOutDocCowEntry(transaction,seasonId,pDoc->GetId(),pEntry);
		entryEnum.advance();
	}

}

void MuuDAO::InsertOutDocCowEntry(Transaction transaction,uint32_t season,uint32_t docId,ICowEntry* pEntry)
{
	InsertCowEntry(transaction,L"outdoc_cows",season,docId,pEntry);
}

bool MuuDAO::DeleteOutDoc(Transaction transaction,uint32_t docId)
{	
	if(DeleteOutDocCowEntries(transaction,docId))
	{
		SQLQuery q;
		q << L"DELETE FROM outdocs where docid = %0";
		q.parse();
		return transaction->Delete(q.str(docId).c_str());
	}
	return false;
}


bool MuuDAO::DeleteInDocCowEntries(Transaction transaction,uint32_t docId)
{
	return DeleteCowEntries(transaction,L"indoc_cows",docId);
}

bool MuuDAO::DeleteBuyDocCowEntries(Transaction transaction,uint32_t docId)
{
	return DeleteCowEntries(transaction,L"buydoc_cows",docId);
}

bool MuuDAO::DeleteMoveDocCowEntries(Transaction transaction,uint32_t docId)
{
	return DeleteCowEntries(transaction,L"movedoc_cows",docId);
}

bool MuuDAO::DeleteSellDocCowEntries(Transaction transaction,uint32_t docId)
{
	return DeleteCowEntries(transaction,L"selldoc_cows",docId);
}

bool MuuDAO::DeleteOutDocCowEntries(Transaction transaction,uint32_t docId)
{
	return DeleteCowEntries(transaction,L"outdoc_cows",docId);
}

bool MuuDAO::DeleteCowEntries(Transaction transaction,const std::wstring& sTable,uint32_t docId)
{
	SQLQuery q;
	q << L"DELETE FROM %0 where doc = %1";
	q.parse();
	return transaction->Delete(q.str(sTable,docId).c_str());
}
	

BuyInvoice* MuuDAO::InsertBuyInvoice(Transaction transaction,uint32_t season,IBuyInvoice* pInvoice)
{

	uint32_t invoiceId = NextSeqValue(transaction,L"buyinvoices_seq");

	InsertInvoice(transaction,L"buy_invoices",season,invoiceId,pInvoice);	

	InsertBuyInvoiceHent(transaction,pInvoice->GetInvoiceHent(),invoiceId,season);

	BuyInvoice* pBuyInvoice = new BuyInvoice(invoiceId,pInvoice->GetInvoiceScope());
	pBuyInvoice->CopyFrom(pInvoice);
	ICowInvoiceEntry* pEntry = NULL;
	try
	{
		PtrEnumerator<ICowInvoiceEntry> entryEnum = pBuyInvoice->EnumEntries();
		
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			InsertBuyInvoiceEntry(transaction,pEntry,invoiceId,season);
			entryEnum.advance();
		}

		IInvoiceDeduction* deduction;
		PtrEnumerator<IInvoiceDeduction> enumDeductions = pBuyInvoice->EnumDeductions();
		while (enumDeductions.hasNext())
		{
			deduction = *enumDeductions;
			InsertBuyInvoiceDeduction(transaction, deduction, invoiceId, season);
			enumDeductions.advance();
		}


	}
	catch(...)
	{
		if(pBuyInvoice)
			delete pBuyInvoice;
		std::rethrow_exception(std::current_exception());
	}
	
	return pBuyInvoice;
}

void MuuDAO::InsertInvoiceHent(Transaction transaction,const std::wstring& sTable,const IInvoiceHent* pHent,uint32_t invoiceId,uint32_t season)
{
	SQLQuery q;
	q << L"INSERT INTO %0 ("
		 L"name,"		//1
		 L"zip, "		//2
		 L"city,"		//3
		 L"street,"		//4
		 L"pobox,"		//5
		 L"fiscal_no,"	//6
		 L"hentno,"		//7
		 L"accountno,"	//8
		 L"bankname,"	//9
		 L"personal_no,"		//10
		 L"personal_id_no,"		//11
		 L"issuepost,"	//12
		 L"issuedate,"	//13
		 L"season,"		//14
		 L"parent_hent,"//15
		 L"invoice"		//16
		 L") VALUES ("
		 L"%1q," 
		 L"%2q," 
		 L"%3q," 
		 L"%4q," 
		 L"%5q," 
		 L"%6q," 
		 L"%7q," 
		 L"%8q," 
		 L"%9q," 
		 L"%10q," 
		 L"%11q," 
		 L"%12q," 
		 L"%13q,"
		 L"%14," 
		 L"%15," 
		 L"%16"
		 L")";
	
	q.parse();

	SQLQueryParms params;

	params  << sTable						//0
			<< pHent->GetName()				//1
			<< pHent->GetZip()				//2
			<< pHent->GetCity()				//3
			<< pHent->GetStreet()			//4
			<< pHent->GetPOBox()			//5
			<< pHent->GetFiscalNo()			//6
			<< pHent->GetHentNo()			//7
			<< pHent->GetAccountNo()		//8
			<< pHent->GetBankName()							//9
			<< pHent->GetPESEL()							//10
			<< pHent->GetIdNo()								//11
			<< pHent->GetIssuePost()						//12
			<< SQLString(pHent->GetIssueDate(),true)		//13
			<< season						//14
			<< pHent->GetParentHentId()		//15
			<< invoiceId;					//16

	transaction->Insert(q.str(params).c_str());
 
}

void MuuDAO::InsertInvoiceEntry(Transaction transaction,const std::wstring& sTable,const ICowInvoiceEntry* pEntry,uint32_t invoiceId,uint32_t season)
{
	SQLQuery q;
	q << L"INSERT INTO %0 ("
		 L"cow,"		//1
		 L"invoice,"	//2
		 L"price,"		//3
		 L"class,"		//4
		 L"weight,"		//5
		 L"season"		//6
		 L") VALUES ("
		 L"%1q," 
		 L"%2,"
		 L"%3,"
		 L"%4,"
		 L"%5,"
		 L"%6"
		 L")";

	q.parse();

	SQLQueryParms params;

	params << sTable  //0
		<< pEntry->GetCowId()		//1
		<< invoiceId				//2
		<< pEntry->GetPrice()		//3
		<< pEntry->GetClassId()		//4
		<< pEntry->GetWeight()		//5
		<< season;					//6

	transaction->Insert(q.str(params).c_str());
}

void MuuDAO::InsertInvoiceDeduction(Transaction transaction, const std::wstring& sTable, const IInvoiceDeduction* pDeduction, uint32_t invoiceId, uint32_t season)
{
	SQLQuery q;
	q << L"INSERT INTO %0 ("
		L"code,"		//1
		L"fraction,"	//2
		L"reason,"		//3		
		L"enabled,"		//4
		L"invoice,"		//5
		L"season"		//6
		L") VALUES ("
		L"%1q,"
		L"%2,"
		L"%3q,"
		L"%4,"
		L"%5,"
		L"%6"
		L")";

	q.parse();

	SQLQueryParms params;

	params << sTable				//0
		<< pDeduction->GetCode()	//1
		<< pDeduction->GetFraction()//2
		<< pDeduction->GetReason()	//3
		<< (pDeduction->GetIsEnabled() ? 1 : 0) //4
		<< invoiceId				//5
		<< season;					//6

	transaction->Insert(q.str(params).c_str());
}

	
void MuuDAO::InsertBuyInvoiceHent(Transaction transaction,const IInvoiceHent* pHent,uint32_t invoiceId,uint32_t season)
{
	InsertInvoiceHent(transaction,L"buy_invoice_hents",pHent,invoiceId,season);	
}

void MuuDAO::InsertBuyInvoiceEntry(Transaction transaction,const ICowInvoiceEntry* pEntry,uint32_t invoiceId,uint32_t season)
{
	InsertInvoiceEntry(transaction,L"buy_invoice_entries",pEntry,invoiceId,season);
}

void MuuDAO::InsertBuyInvoiceDeduction(Transaction transaction, const IInvoiceDeduction* pDeduction, uint32_t invoiceId, uint32_t season)
{
	InsertInvoiceDeduction(transaction, L"buy_invoice_deductions", pDeduction, invoiceId, season);
}

void MuuDAO::InsertSellInvoiceHent(Transaction transaction,const IInvoiceHent* pHent,uint32_t invoiceId,uint32_t season)
{
	InsertInvoiceHent(transaction,L"sell_invoice_hents",pHent,invoiceId,season);
}

void MuuDAO::InsertSellInvoiceEntry(Transaction transaction,const ICowInvoiceEntry* pEntry,uint32_t invoiceId,uint32_t season)
{
	InsertInvoiceEntry(transaction,L"sell_invoice_entries",pEntry,invoiceId,season);
}

void MuuDAO::InsertSellInvoiceDeduction(Transaction transaction, const IInvoiceDeduction* pDeduction, uint32_t invoiceId, uint32_t season)
{
	InsertInvoiceDeduction(transaction, L"sell_invoice_deductions", pDeduction, invoiceId, season);
}

void MuuDAO::UpdateBuyDocWithInvoice(Transaction transaction,uint32_t docId,uint32_t invoiceId)
{
	SQLQuery q;
	q << L"UPDATE buydocs SET invoice = %0 WHERE docid = %1";
	q.parse();
	transaction->Update(q.str(SQLIdable(invoiceId),docId).c_str());
}

void MuuDAO::UpdateSellDocWithInvoice(Transaction transaction,uint32_t docId,uint32_t invoiceId)
{
	SQLQuery q;
	q << L"UPDATE selldocs SET invoice = %0 WHERE docid = %1";
	q.parse();
	transaction->Update(q.str(SQLIdable(invoiceId),docId).c_str());
}

void  MuuDAO::EnsureCounter(Transaction transaction, const std::wstring& sCounterName, uint32_t season)
{
	try
	{
		GetCounterValue(transaction, sCounterName, season);
	}
	catch (SQLException& e)
	{
		CreateCounter(transaction, sCounterName, season);
	}
}

void MuuDAO::SetCounterValue(Transaction transaction,uint32_t value,const std::wstring& sCounterName,uint32_t season)
{
	SQLQuery stmt;
	stmt << L"UPDATE counters SET value = %2 WHERE name=%0q AND season=%1";
	stmt.parse();	
	
	transaction->Update(stmt.str(sCounterName,season,value).c_str());	
}


uint32_t MuuDAO::GetCounterValue(Transaction transaction, const std::wstring& sCounterName, uint32_t season)
{
	SQLQuery stmt;
	stmt << L"SELECT value FROM counters WHERE name=%0q AND season=%1";
	stmt.parse();

	Cursor cursor = transaction->Query(stmt.str(sCounterName, season).c_str());

	if (cursor.nextRow())
	{
		return cursor.getColUInt(0);
	}

	BOOST_THROW_EXCEPTION(SQLException(TextUtils::FormatA(L"there is no counter named %s defined in season with id = %d", sCounterName.c_str(), season)));
}

uint32_t MuuDAO::NextCounterValue(Transaction transaction,const std::wstring& sCounterName,uint32_t season)
{	
	uint32_t result = GetCounterValue(transaction, sCounterName, season)+1;
	SetCounterValue(transaction,result,sCounterName,season);
	return result;
		
}

void MuuDAO::CreateCounter(Transaction transaction,const std::wstring& sCounterName,uint32_t season)
{
	SQLQuery stmt;
	stmt << L"INSERT INTO counters (name, \"value\", season) VALUES (%0q, 0, %1)";
	stmt.parse();
	transaction->Insert(stmt.str(sCounterName,season).c_str());
	transaction->Commit();
}


bool MuuDAO::DeleteInvoiceHent(Transaction transaction,const std::wstring& sTable,uint32_t invoiceId)
{
	SQLQuery q;
	q << L"DELETE FROM %0 where invoice = %1";
	q.parse();
	return transaction->Delete(q.str(sTable,invoiceId).c_str());
}

bool MuuDAO::DeleteBuyInvoiceHent(Transaction transaction,uint32_t invoiceId)
{
	return DeleteInvoiceHent(transaction,L"buy_invoice_hents",invoiceId);
}

bool MuuDAO::DeleteBuyInvoice(Transaction transaction,uint32_t invoiceId)
{
	if (DeleteBuyInvoiceCowEntries(transaction, invoiceId) && DeleteBuyInvoiceHent(transaction, invoiceId))
	{
		DeleteBuyInvoiceDeductions(transaction, invoiceId);
		SQLQuery q;
		q << L"DELETE FROM buy_invoices where invoiceid = " << invoiceId;
		return transaction->Delete(q.str(invoiceId).c_str());
	}
	return false;
}

bool MuuDAO::DeleteBuyInvoiceCowEntries(Transaction transaction,uint32_t invoiceId)
{
	return DeleteInvoiceCowEntries(transaction,L"buy_invoice_entries",invoiceId);
}

bool MuuDAO::DeleteInvoiceCowEntries(Transaction transaction,const std::wstring& sTable,uint32_t invoiceId)
{
	SQLQuery q;
	q << L"DELETE FROM %0 where invoice = %1";
	q.parse();
	return transaction->Delete(q.str(sTable,invoiceId).c_str());
}

void MuuDAO::UpdateBuyInvoice(Transaction transaction,IBuyInvoice* pInvoice)
{
	SQLQuery seasonQ;
	seasonQ << L"SELECT season from buy_invoices WHERE invoiceid = " << pInvoice->GetId();
	
	uint32_t seasonId = transaction->QueryScalar(seasonQ.str(),NULL_ID);

	SQLQuery stmt;
	stmt << L"UPDATE buy_invoices SET "
			L"invoicedate = %1q,"		//1
			L"paid = %2q,"				//2
			L"invoice_type = %3q,"		//3
			L"vatrate = %4,"			//4
			L"payway = %5q,"			//5
			L"payduedays = %6,"			//6
			L"transactionplace = %7n,"	//7
			L"transactiondate = %8q,"	//8
			L"customnumber = %9n,"		//9	
			L"extras = %10n"			//10		 
			L" WHERE invoiceid = %0;";  //0
		
	stmt.parse();

	SQLQueryParms params;

	params  << pInvoice->GetId()								//0
			<< SQLString(pInvoice->GetInvoiceDate(),true)		//1
			<< SQLString(pInvoice->GetPaidDate(),true)			//2
			<< pInvoice->GetInvoiceType()						//3
			<< pInvoice->GetVATRate()							//4
			<< pInvoice->GetPayWay()							//5
			<< pInvoice->GetPayDueDays()						//6
			<< pInvoice->GetTransactionPlace()					//7
			<< SQLString(pInvoice->GetTransactionDate(),true)	//8
			<< pInvoice->GetCustomNumber()						//9
			<< pInvoice->GetExtras()							//10
			;

	transaction->Update(stmt.str(params).c_str());
	
	DeleteBuyInvoiceHent(transaction,pInvoice->GetId());
	DeleteBuyInvoiceCowEntries(transaction,pInvoice->GetId());
	DeleteBuyInvoiceDeductions(transaction, pInvoice->GetId());

	PtrEnumerator<ICowInvoiceEntry> entryEnum = pInvoice->EnumEntries();
	ICowInvoiceEntry* pEntry;
	while(entryEnum.hasNext())
	{
		pEntry = *entryEnum;
		InsertBuyInvoiceEntry(transaction,pEntry,pInvoice->GetId(),seasonId);
		entryEnum.advance();
	}

	InsertBuyInvoiceHent(transaction,pInvoice->GetInvoiceHent(),pInvoice->GetId(),seasonId);

	IInvoiceDeduction* deduction;
	PtrEnumerator<IInvoiceDeduction> enumDeductions = pInvoice->EnumDeductions();
	while (enumDeductions.hasNext())
	{
		deduction = *enumDeductions;
		InsertBuyInvoiceDeduction(transaction, deduction, pInvoice->GetId(), seasonId);
		enumDeductions.advance();
	}

}

void MuuDAO::UpdateCowWithBuyInvoice(Transaction transaction,uint32_t cowId,uint32_t invoiceId)
{
	SQLQuery q;
	q << L"UPDATE cattle SET buy_invoice = %0 WHERE animalid = %1";
	q.parse();
	transaction->Update(q.str(SQLIdable(invoiceId),cowId).c_str());
}

void MuuDAO::UpdateSellInvoice(Transaction transaction,ISellInvoice* pInvoice)
{
	SQLQuery seasonQ;
	seasonQ << L"SELECT season from sell_invoices WHERE invoiceid = " << pInvoice->GetId();
	
	uint32_t seasonId = transaction->QueryScalar(seasonQ.str(),NULL_ID);

	SQLQuery stmt;
	stmt << L"UPDATE sell_invoices SET "
			L"invoicedate = %1q,"		//1
			L"paid = %2q,"				//2
			L"invoice_type = %3q,"		//3
			L"vatrate = %4,"				//4
			L"payway = %5q,"				//5
			L"payduedays = %6,"			//6
			L"transactionplace = %7n,"	//7
			L"transactiondate = %8q,"	//8
			L"customnumber = %9n,"		//9	
			L"extras = %10n"			//10		 
			L" WHERE invoiceid = %0;";   //0
		
	stmt.parse();

	SQLQueryParms params;

	params  << pInvoice->GetId()								//0
			<< SQLString(pInvoice->GetInvoiceDate(),true)		//1
			<< SQLString(pInvoice->GetPaidDate(),true)			//2
			<< pInvoice->GetInvoiceType()						//3
			<< pInvoice->GetVATRate()							//4
			<< pInvoice->GetPayWay()							//5
			<< pInvoice->GetPayDueDays()						//6
			<< pInvoice->GetTransactionPlace()					//7
			<< SQLString(pInvoice->GetTransactionDate(),true)	//8
			<< pInvoice->GetCustomNumber()						//9
			<< pInvoice->GetExtras()							//10
			;

	transaction->Update(stmt.str(params).c_str());
	
	DeleteSellInvoiceHent(transaction,pInvoice->GetId());
	DeleteSellInvoiceCowEntries(transaction,pInvoice->GetId());
	DeleteSellInvoiceDeductions(transaction, pInvoice->GetId());

	PtrEnumerator<ICowInvoiceEntry> entryEnum = pInvoice->EnumEntries();
	ICowInvoiceEntry* pEntry;
	while(entryEnum.hasNext())
	{
		pEntry = *entryEnum;
		InsertSellInvoiceEntry(transaction,pEntry,pInvoice->GetId(),seasonId);
		entryEnum.advance();
	}

	InsertSellInvoiceHent(transaction,pInvoice->GetInvoiceHent(),pInvoice->GetId(),seasonId);

	IInvoiceDeduction* deduction;
	PtrEnumerator<IInvoiceDeduction> enumDeductions = pInvoice->EnumDeductions();
	while (enumDeductions.hasNext())
	{
		deduction = *enumDeductions;
		InsertSellInvoiceDeduction(transaction, deduction, pInvoice->GetId(), seasonId);
		enumDeductions.advance();
	}

}

void MuuDAO::UpdateCowWithSellInvoice(Transaction transaction,uint32_t cowId,uint32_t invoiceId)
{
	SQLQuery q;
	q << L"UPDATE cattle SET sell_invoice = %0 WHERE animalid = %1";
	q.parse();
	transaction->Update(q.str(SQLIdable(invoiceId),cowId).c_str());
}

void MuuDAO::InsertInvoice(Transaction transaction,const std::wstring& sTable,uint32_t season,uint32_t invoiceId,IInvoice* pInvoice)
{
	SQLQuery q;
	q << L"INSERT INTO %0 ("
		 L"invoiceno,"		//1
		 L"extras,"			//2
		 L"invoicedate,"		//3
		 L"paid,"			//4
		 L"correct_for,"		//5
		 L"invoice_type,"	//6
		 L"vatrate,"			//7
		 L"payway,"			//8
		 L"payduedays,"		//9
		 L"transactionplace,"//10
		 L"transactiondate,"	//11		
		 L"customnumber,"	//12
		 L"invoicescope,"	//13
		 L"invoiceid,"		//14
		 L"season"			//15
		 L") VALUES ("
		 L"%1,"
		 L"%2n,"
		 L"%3," 
		 L"%4," 
		 L"%5," 
		 L"%6q," 
		 L"%7,"
		 L"%8q," 
		 L"%9," 
		 L"%10n,"
		 L"%11n,"
		 L"%12n,"
		 L"%13q,"
		 L"%14,"
		 L"%15"
		 L")";	

	q.parse();
	
	SQLQueryParms params;

	params  << sTable											//0
			<< pInvoice->GetInvoiceNo()							//1
			<< pInvoice->GetExtras()							//2
			<< SQLString(pInvoice->GetInvoiceDate(),true)		//3
			<< SQLString(pInvoice->GetPaidDate(),true)			//4
			<< SQLIdable(pInvoice->GetCorrectFor())				//5
			<< pInvoice->GetInvoiceType()						//6
			<< pInvoice->GetVATRate()							//7
			<< pInvoice->GetPayWay()							//8
			<< pInvoice->GetPayDueDays()						//9
			<< pInvoice->GetTransactionPlace()					//10
			<< SQLString(pInvoice->GetTransactionDate(),true)	//11
			<< pInvoice->GetCustomNumber()						//12
			<< pInvoice->GetInvoiceScope()						//13
			<< invoiceId										//14
			<< season;											//15				

	transaction->Insert(q.str(params).c_str());
}

SellInvoice* MuuDAO::InsertSellInvoice(Transaction transaction,uint32_t season,ISellInvoice* pInvoice)
{
	uint32_t invoiceId = NextSeqValue(transaction,L"sellinvoices_seq");

	InsertInvoice(transaction,L"sell_invoices",season,invoiceId,pInvoice);	

	InsertSellInvoiceHent(transaction,pInvoice->GetInvoiceHent(),invoiceId,season);

	SellInvoice* pSellInvoice = new SellInvoice(invoiceId,pInvoice->GetInvoiceScope());
	pSellInvoice->CopyFrom(pInvoice);
	ICowInvoiceEntry* pEntry = NULL;
	try
	{
		PtrEnumerator<ICowInvoiceEntry> entryEnum = pSellInvoice->EnumEntries();
		
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			InsertSellInvoiceEntry(transaction,pEntry,invoiceId,season);
			entryEnum.advance();
		}

		IInvoiceDeduction* deduction;
		PtrEnumerator<IInvoiceDeduction> enumDeductions = pSellInvoice->EnumDeductions();
		while (enumDeductions.hasNext())
		{
			deduction = *enumDeductions;
			InsertSellInvoiceDeduction(transaction, deduction, invoiceId, season);
			enumDeductions.advance();
		}


	}
	catch(...)
	{
		if(pSellInvoice)
			delete pSellInvoice;
		std::rethrow_exception(std::current_exception());
	}
	
	return pSellInvoice;
}

bool MuuDAO::DeleteSellInvoice(Transaction transaction,uint32_t invoiceId)
{
	if (DeleteSellInvoiceCowEntries(transaction, invoiceId) && DeleteSellInvoiceHent(transaction, invoiceId))
	{
		DeleteSellInvoiceDeductions(transaction, invoiceId);
		SQLQuery q;
		q << L"DELETE FROM sell_invoices where invoiceid = " << invoiceId;
		return transaction->Delete(q.str(invoiceId).c_str());
	}
	return false;

}

bool MuuDAO::DeleteSellInvoiceCowEntries(Transaction transaction,uint32_t invoiceId)
{
	return DeleteInvoiceCowEntries(transaction,L"sell_invoice_entries",invoiceId);
}

bool MuuDAO::DeleteSellInvoiceHent(Transaction transaction,uint32_t invoiceId)
{
	return DeleteInvoiceHent(transaction,L"sell_invoice_hents",invoiceId);
}

bool MuuDAO::DeleteBuyInvoiceDeductions(Transaction transaction, uint32_t invoiceId)
{
	return DeleteInvoiceDeductions(transaction, L"buy_invoice_deductions", invoiceId);
}

bool MuuDAO::DeleteSellInvoiceDeductions(Transaction transaction, uint32_t invoiceId)
{
	return DeleteInvoiceDeductions(transaction, L"sell_invoice_deductions", invoiceId);
}

bool MuuDAO::DeleteInvoiceDeductions(Transaction transaction, const std::wstring& sTable, uint32_t invoiceId)
{
	SQLQuery q;
	q << L"DELETE FROM %0 where invoice = %1";
	q.parse();
	return transaction->Delete(q.str(sTable, invoiceId).c_str());
}

bool MuuDAO::LoadBuyInvoiceDeductions(InvoiceDeductionsMap& result, uint32_t seasonId, SimpleProgress *pProgress)
{
	return LoadInvoicesDeductionsFromTable(L"buy_invoice_deductions", result, seasonId, pProgress);
}

bool MuuDAO::LoadSellInvoiceDeductions(InvoiceDeductionsMap& result, uint32_t seasonId, SimpleProgress *pProgress)
{
	return LoadInvoicesDeductionsFromTable(L"sell_invoice_deductions", result, seasonId, pProgress);
}

bool MuuDAO::LoadInvoicesDeductionsFromTable(const std::wstring& sTableName, InvoiceDeductionsMap& result, uint32_t seasonId, SimpleProgress *pProgress)
{
	SQLQuery q;
	Transaction transaction = OpenTransaction();
	SafeSimpleProgress progress(pProgress);

	q << L"SELECT COUNT(*) FROM %1 WHERE season = %0";
	q.parse();

	int steps = transaction->QueryScalar(q.str(seasonId, sTableName), 0);
	if (steps < 0) return false;

	progress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM %1 WHERE season = %0";
	q.parse();

	Cursor cursor = transaction->Query(q.str(seasonId, sTableName));

	return ExtractInvoiceDeductions(cursor, result, &progress);
}

bool MuuDAO::ExtractInvoiceDeductions(Cursor& cursor, InvoiceDeductionsMap& result, SimpleProgress *pProgress /*= NULL*/)
{
	InvoiceDeduction* oneRow = NULL;
	SafeSimpleProgress progress(pProgress);

	int colCODE, colFRACTION, colREASON, colENABLED, colINVOICE;

	colCODE = cursor.colIndex(L"CODE");
	colFRACTION = cursor.colIndex(L"FRACTION");
	colREASON = cursor.colIndex(L"REASON");
	colENABLED = cursor.colIndex(L"ENABLED");
	colINVOICE = cursor.colIndex(L"INVOICE");
	
	try
	{
		while (cursor.nextRow())
		{
			oneRow = new InvoiceDeduction();

			oneRow->SetCode(cursor.getColString(colCODE));
			oneRow->SetFraction(cursor.getColDecimal(colFRACTION,Decimal(0L)));
			oneRow->SetReason(cursor.getColString(colREASON));
			oneRow->SetIsEnabled(cursor.getColInt(colENABLED) != 0);

			result.insert(std::pair<uint32_t, InvoiceDeduction*>(cursor.getColInt(colINVOICE), oneRow));

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


bool MuuDAO::LoadBankAccounts(NewPtrFlushArray<BankAccount>& result, uint32_t farmId, SimpleProgress *pProgress/* = NULL*/)
{
	BankAccount *onerow = NULL;
	
	int colACCOUNTID, colALIAS, colACCOUNTNO, colBANKNAME;

	SQLQuery q;

	int step = 0;
	q << L"SELECT COUNT(*) FROM farm_bank_accounts WHERE farm = %0";
	q.parse();

	Transaction transaction = OpenTransaction();

	int steps = transaction->QueryScalar(q.str(farmId), 0);
	if (steps < 0) return false;
	SafeSimpleProgress progress(pProgress);

	progress.SetSteps(steps);

	q.reset();
	q << L"SELECT * FROM farm_bank_accounts WHERE farm = %0";
	q.parse();

	try
	{
		Cursor cursor = transaction->Query(q.str(farmId));

		colACCOUNTID = cursor.colIndex(L"ACCOUNT_ID");
		colALIAS = cursor.colIndex(L"ALIAS");
		colACCOUNTNO = cursor.colIndex(L"ACCOUTNO");
		colBANKNAME = cursor.colIndex(L"BANK_NAME");
	
		while (cursor.nextRow())
		{
			onerow = new BankAccount(cursor.getColInt(colACCOUNTID));
			onerow->SetAlias(cursor.getColString(colALIAS));
			onerow->SetBankName(cursor.getColString(colBANKNAME));
			onerow->SetAccountNo(AccountNoFactory::FromString(cursor.getColString(colACCOUNTNO)));
			
			result.Add(onerow);

			onerow = NULL;

			step++;
			if (!progress.Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if (onerow) delete onerow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}