
#SET(CMAKE_LEGACY_CYGWIN_WIN32 0)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)

PROJECT(BkServer)

SET(SRC_ROOT ${CMAKE_SOURCE_DIR}/../src )

SET (SRCS ${SRC_ROOT}/main.cpp
	${SRC_ROOT}/BKService.cpp
	${SRC_ROOT}/cloud/CloudPublisher.cpp
	${SRC_ROOT}/cloud/ChangePublisher.cpp
	${SRC_ROOT}/cloud/FirebaseTopicPublisher.cpp
	${SRC_ROOT}/cloud/GoogleServiceAccount.cpp
	${SRC_ROOT}/cloud/Auth0Account.cpp
	${SRC_ROOT}/cloud/Auth0TokenProvider.cpp
	${SRC_ROOT}/cloud/HttpClient.cpp
	${SRC_ROOT}/BKServiceConstructor.cpp
	${SRC_ROOT}/BKServiceDestructor.cpp
	${SRC_ROOT}/ClientSeasonSession.cpp
	${SRC_ROOT}/ClientSeasonSessionFacade.cpp
	${SRC_ROOT}/ClientSession.cpp
	${SRC_ROOT}/BkLogger.cpp
	${SRC_ROOT}/CowNoReference.cpp
	${SRC_ROOT}/dao/BaseDAO.cpp
	${SRC_ROOT}/db/pgserver/PGSvrPool.cpp
	${SRC_ROOT}/db/DbConnection.cpp
	${SRC_ROOT}/db/DbConnectionOptions.cpp
	${SRC_ROOT}/db/Result.cpp
	${SRC_ROOT}/db/Cursor.cpp
	${SRC_ROOT}/dao/MuuDAO.cpp
	${SRC_ROOT}/db/query/mysql_func.cpp
	${SRC_ROOT}/db/query/SQLQuery.cpp
	${SRC_ROOT}/db/query/sql_string.cpp
	${SRC_ROOT}/db/SQLSingleConnectionPool.cpp
	${SRC_ROOT}/db/Transaction.cpp
	${SRC_ROOT}/FarmAudience.cpp
	${SRC_ROOT}/FarmSession.cpp
	${SRC_ROOT}/FarmSessionBuilder.cpp
	${SRC_ROOT}/FarmSessionFinalizer.cpp
	${SRC_ROOT}/IFetchHandle.cpp
	${SRC_ROOT}/InvoiceRelations.cpp
	${SRC_ROOT}/Log.cpp	
	${SRC_ROOT}/object/LockableObject.cpp
	${SRC_ROOT}/SeasonObserver.cpp
	${SRC_ROOT}/SeasonSession.cpp
	${SRC_ROOT}/SeasonSessionBuilder.cpp
	${SRC_ROOT}/SeasonSessionFinalizer.cpp
	${SRC_ROOT}/SeasonSpectator.cpp
	${SRC_ROOT}/SessionLockRecord.cpp
	${SRC_ROOT}/seasonimpl/ClientSeasonSessionInterfaceImpl.cpp
	${SRC_ROOT}/seasonimpl/FetchHandleImpl.cpp
	${SRC_ROOT}/seasonimpl/ScopedUpdateState.cpp
	${SRC_ROOT}/seasonimpl/SeasonSessionFacadeImpl.cpp
	${SRC_ROOT}/seasonimpl/StateHolder.cpp
	${SRC_ROOT}/seasonimpl/operations/InsertPurchaseOperation.cpp						
	${SRC_ROOT}/seasonimpl/operations/LoggerAwareSeasonOperation.cpp
	${SRC_ROOT}/seasonimpl/operations/SeasonOperation.cpp
	${SRC_ROOT}/index/HentsIndex.cpp
	${SRC_ROOT}/settings/serializers/IntegerSettingSerializer.cpp
	${SRC_ROOT}/settings/serializers/IntToIntMapSerializer.cpp
	${SRC_ROOT}/settings/serializers/UIntegerSettingSerializer.cpp
	${SRC_ROOT}/settings/serializers/DecimalSettingSerializer.cpp
	${SRC_ROOT}/settings/serializers/BooleanSettingSerializer.cpp
	${SRC_ROOT}/settings/serializers/IntegerVectorSerializer.cpp
	${SRC_ROOT}/settings/SettingDefinition.cpp
	${SRC_ROOT}/settings/SettingPart.cpp
	${SRC_ROOT}/settings/SettingType.cpp
	${SRC_ROOT}/settings/SettingValue.cpp
	${SRC_ROOT}/settings/UserSettingsManager.cpp
	${SRC_ROOT}/settings/UserSettings.cpp
	${SRC_ROOT}/snapshot/DocCowItem.cpp
	${SRC_ROOT}/snapshot/DocSnapshot.cpp
	${SRC_ROOT}/snapshot/HentsDocSnapshot.cpp
	${SRC_ROOT}/snapshot/InvoiceCowItem.cpp
	${SRC_ROOT}/snapshot/InvoiceSnapshot.cpp
	${SRC_ROOT}/snapshot/MoveDocSnapshot.cpp
	${SRC_ROOT}/snapshot/OwnDocSnapshot.cpp
	${SRC_ROOT}/stdafx.cpp
	${SRC_ROOT}/transaction/BuyInvoiceCowsTransaction.cpp
	${SRC_ROOT}/transaction/BuyInvoiceDocsTransaction.cpp
	${SRC_ROOT}/transaction/DocTransactionCowItem.cpp
	${SRC_ROOT}/transaction/DocTransactionSkeleton.cpp
	${SRC_ROOT}/transaction/EditBuyDocTransaction.cpp
	${SRC_ROOT}/transaction/EditInDocTransaction.cpp
	${SRC_ROOT}/transaction/EditMoveDocTransaction.cpp
	${SRC_ROOT}/transaction/EditSellDocTransaction.cpp
	${SRC_ROOT}/transaction/IClientTransaction.cpp
	${SRC_ROOT}/transaction/InvoiceTransactionCowItem.cpp
	${SRC_ROOT}/transaction/InvoiceTransactionSkeleton.cpp
	${SRC_ROOT}/transaction/NewBuyDocTransaction.cpp
	${SRC_ROOT}/transaction/NewInDocTransaction.cpp
	${SRC_ROOT}/transaction/NewMoveDocTransaction.cpp
	${SRC_ROOT}/transaction/NewSellDocTransaction.cpp
	${SRC_ROOT}/transaction/OutDocTransaction.cpp
	${SRC_ROOT}/transaction/PlusDocTransactionSkeleton.cpp
	${SRC_ROOT}/transaction/SellInvoiceCowsTransaction.cpp
	${SRC_ROOT}/transaction/SellInvoiceDocsTransaction.cpp
	${SRC_ROOT}/transaction/DeductionTransactionItem.cpp
	${SRC_ROOT}/transaction/purchase/DownloadPurchaseTransaction.cpp
	${SRC_ROOT}/transaction/purchase/AgentIntegrityProblem.cpp
	${SRC_ROOT}/transaction/purchase/AgentIntegritySolution.cpp
	${SRC_ROOT}/transaction/purchase/AgentProblemDetectingVisitor.cpp
	${SRC_ROOT}/transaction/purchase/ClassIntegrityProblem.cpp
	${SRC_ROOT}/transaction/purchase/ClassIntegritySolution.cpp
	${SRC_ROOT}/transaction/purchase/ClassProblemDetectingVisitor.cpp
	${SRC_ROOT}/transaction/purchase/FirstOwnerHentIntegrityProblem.cpp
	${SRC_ROOT}/transaction/purchase/FirstOwnerHentProblemDetectingVisitor.cpp
	${SRC_ROOT}/transaction/purchase/FstOwnerHentIntegritySolution.cpp
	${SRC_ROOT}/transaction/purchase/HerdIntegrityProblem.cpp
	${SRC_ROOT}/transaction/purchase/HerdIntegritySolution.cpp
	${SRC_ROOT}/transaction/purchase/HerdProblemDetectingVisitor.cpp
	${SRC_ROOT}/transaction/purchase/IntegrityProblem.cpp
	${SRC_ROOT}/transaction/purchase/IntegrityProblemIdentifier.cpp
	${SRC_ROOT}/transaction/purchase/InvoiceHentIntegrityProblem.cpp
	${SRC_ROOT}/transaction/purchase/InvoiceHentIntegritySolution.cpp
	${SRC_ROOT}/transaction/purchase/InvoiceHentProblemDetectingVisitor.cpp
	${SRC_ROOT}/transaction/purchase/ProblemVerifingVisitor.cpp
	${SRC_ROOT}/transaction/purchase/PurchaseTraverser.cpp
	${SRC_ROOT}/transaction/purchase/SolutionApplyingVisitor.cpp
	${SRC_ROOT}/transaction/purchase/StockIntegrityProblem.cpp
	${SRC_ROOT}/transaction/purchase/StockIntegritySolution.cpp
	${SRC_ROOT}/transaction/purchase/StockProblemDetectingVisitor.cpp
	${SRC_ROOT}/transaction/purchase/InStockCowDuplicationSolution.cpp
	${SRC_ROOT}/transaction/purchase/InStockCowDuplictionProblemDetectingVisitor.cpp
	${SRC_ROOT}/transaction/purchase/InStockCowDuplicationProblem.cpp
	${SRC_ROOT}/transaction/TransferBuyDocTransaction.cpp
	${SRC_ROOT}/repo/CursorDescriptor.cpp
	${SRC_ROOT}/repo/RepoClient.cpp
	${SRC_ROOT}/repo/RepoClientFactory.cpp
	${SRC_ROOT}/log/XLogger.cpp
	${SRC_ROOT}/log/LoggerAware.cpp
	${SRC_ROOT}/UserSession.cpp
	${SRC_ROOT}/UserSessionBuilder.cpp
	${SRC_ROOT}/dao/UserDAO.cpp
	${SRC_ROOT}/BKServer.cpp
	)
	
 SET(COMMON_ROOT ${CMAKE_SOURCE_DIR}/../../common)
 
 SET (COMMON_SRCS	${COMMON_ROOT}/compare/TypesCompare.cpp
					${COMMON_ROOT}/configuration/print/PrintProfile.cpp
					${COMMON_ROOT}/configuration/Setting.cpp
					${COMMON_ROOT}/data/IInvoice.cpp
					${COMMON_ROOT}/data/DocKey.cpp					
					${COMMON_ROOT}/data/henttypes/HentTypes.cpp
					${COMMON_ROOT}/data/invoicetypes/InvoiceTypes.cpp
					${COMMON_ROOT}/data/invoicescopes/InvoiceScopes.cpp
					${COMMON_ROOT}/data/payways/PayWays.cpp
					${COMMON_ROOT}/data/impl/Agent.cpp
					${COMMON_ROOT}/data/impl/BuyDoc.cpp
					${COMMON_ROOT}/data/impl/BuyInvoice.cpp
					${COMMON_ROOT}/data/impl/Class.cpp
					${COMMON_ROOT}/data/impl/Cow.cpp
					${COMMON_ROOT}/data/impl/CowDetails.cpp
					${COMMON_ROOT}/data/impl/CowEntry.cpp
					${COMMON_ROOT}/data/impl/CowEntryDetails.cpp
					${COMMON_ROOT}/data/impl/CowInvoiceEntry.cpp
					${COMMON_ROOT}/data/impl/CowInvoiceEntryDetails.cpp
					${COMMON_ROOT}/data/impl/DocContent.cpp
					${COMMON_ROOT}/data/impl/DocDetails.cpp
					${COMMON_ROOT}/data/impl/Farm.cpp
					${COMMON_ROOT}/data/impl/Hent.cpp
					${COMMON_ROOT}/data/impl/Herd.cpp
					${COMMON_ROOT}/data/impl/InDoc.cpp
					${COMMON_ROOT}/data/impl/InvoiceContent.cpp
					${COMMON_ROOT}/data/impl/InvoiceDetails.cpp
					${COMMON_ROOT}/data/impl/InvoiceHent.cpp
					${COMMON_ROOT}/data/impl/MoveDoc.cpp
					${COMMON_ROOT}/data/impl/OutDoc.cpp
					${COMMON_ROOT}/data/impl/Season.cpp
					${COMMON_ROOT}/data/impl/SellDoc.cpp
					${COMMON_ROOT}/data/impl/SellInvoice.cpp
					${COMMON_ROOT}/data/impl/InvoiceDeduction.cpp
					${COMMON_ROOT}/data/impl/InvoiceDeductions.cpp
					${COMMON_ROOT}/data/impl/BankAccount.cpp
					${COMMON_ROOT}/data/impl/serialization/DataSerialization.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableAgent.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableBuyDoc.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableBuyInvoice.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableClass.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableCow.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableCowDetails.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableCowEntry.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableCowEntryDetails.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableCowInvoiceEntry.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableCowInvoiceEntryDetails.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableDocContent.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableDocDetails.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableFarm.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableHent.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableHerd.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableInDoc.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableInvoiceContent.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableInvoiceDetails.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableInvoiceHent.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableMoveDoc.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableOutDoc.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableSeason.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableSellDoc.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableSellInvoice.cpp
					${COMMON_ROOT}/data/impl/serialization/SerializableStock.cpp
					${COMMON_ROOT}/data/impl/serialization/InvoiceDeductionsSerializer.cpp
					${COMMON_ROOT}/data/impl/serialization/InvoiceDeductionSerializer.cpp
					${COMMON_ROOT}/data/impl/serialization/BankAccountSerializer.cpp
					${COMMON_ROOT}/data/impl/Stock.cpp
					${COMMON_ROOT}/data/impl/User.cpp
					${COMMON_ROOT}/data/repo/PurchaseCow.cpp
					${COMMON_ROOT}/data/repo/Purchase.cpp
					${COMMON_ROOT}/data/repo/PurchaseHent.cpp
					${COMMON_ROOT}/data/repo/PurchaseInvoice.cpp
					${COMMON_ROOT}/data/repo/PurchaseInvoiceDeduction.cpp
					${COMMON_ROOT}/data/repo/PurchaseObjPath.cpp
					${COMMON_ROOT}/data/repo/Purchases.cpp
					${COMMON_ROOT}/data/repo/PurchaseHeader.cpp
					${COMMON_ROOT}/data/repo/PurchaseHeaders.cpp
					${COMMON_ROOT}/data/repo/serialization/RepoTypesSerialization.cpp
					${COMMON_ROOT}/data/repo/serialization/SerializablePurchaseCow.cpp
					${COMMON_ROOT}/data/repo/serialization/SerializablePurchase.cpp
					${COMMON_ROOT}/data/repo/serialization/PurchaseHeaderSerializer.cpp
					${COMMON_ROOT}/data/repo/serialization/SerializablePurchaseHent.cpp
					${COMMON_ROOT}/data/repo/serialization/SerializablePurchaseInvoice.cpp
					${COMMON_ROOT}/data/repo/serialization/SerializablePurchaseInvoiceDeduction.cpp
					${COMMON_ROOT}/data/InvoiceKey.cpp
					${COMMON_ROOT}/data/motives/Motives.cpp
					${COMMON_ROOT}/data/runtime/InvoiceItemKey.cpp
					${COMMON_ROOT}/data/runtime/ItemId.cpp
					${COMMON_ROOT}/data/runtime/OperationArguments.cpp
					${COMMON_ROOT}/data/runtime/PurchaseDownloadIssue.cpp
					${COMMON_ROOT}/data/runtime/PurchaseDownloadIssues.cpp
					${COMMON_ROOT}/data/runtime/SolutionParameters.cpp
					${COMMON_ROOT}/data/runtime/SimpleUser.cpp
					${COMMON_ROOT}/data/runtime/PurchaseDownloadIssueCategory.cpp
					${COMMON_ROOT}/data/runtime/serialization/RuntimeTypesSerialization.cpp
					${COMMON_ROOT}/data/runtime/serialization/SerializableOperationArguments.cpp
					${COMMON_ROOT}/data/runtime/serialization/PurchaseDownloadIssuesSerializer.cpp
					${COMMON_ROOT}/data/runtime/serialization/SolutionParametersSerializer.cpp
					${COMMON_ROOT}/data/runtime/serialization/PurchaseDownloadIssueSerializer.cpp
					${COMMON_ROOT}/data/runtime/serialization/PurchaseDownloadIssueCategorySerializer.cpp
					${COMMON_ROOT}/data/runtime/serialization/PurchaseDownloadIssuesSerializer.cpp
					${COMMON_ROOT}/data/types/AccountNo.cpp
					${COMMON_ROOT}/data/types/countries/Countries.cpp
					${COMMON_ROOT}/data/types/countries/Country.cpp
					${COMMON_ROOT}/data/types/CowNo.cpp
					${COMMON_ROOT}/data/types/CowSex.cpp
					${COMMON_ROOT}/data/types/FarmNo.cpp
					${COMMON_ROOT}/data/types/HentNo.cpp
					${COMMON_ROOT}/data/types/HerdNo.cpp
					${COMMON_ROOT}/data/types/serialization/TypesSerialization.cpp
					${COMMON_ROOT}/data/utils/GetDocHerd.cpp
					${COMMON_ROOT}/data/utils/GetDocInvoice.cpp
					${COMMON_ROOT}/errortoken/ErrorToken.cpp
					${COMMON_ROOT}/filesystem/files.cpp
					${COMMON_ROOT}/filesystem/paths.cpp
					${COMMON_ROOT}/logic/changedetect/DocChangeDetector.cpp
					${COMMON_ROOT}/logic/changedetect/InvoiceChangeDetector.cpp
					${COMMON_ROOT}/logic/cowtrail/CowPath.cpp
					${COMMON_ROOT}/logic/cowtrail/CowTracker.cpp
					${COMMON_ROOT}/logic/cowtrail/CowTrail.cpp
					${COMMON_ROOT}/logic/herdregistry/CowPresenceKeys.cpp					
					${COMMON_ROOT}/logic/herdregistry/CowPresenceKey.cpp
					${COMMON_ROOT}/logic/herdregistry/HerdRegistry.cpp
					${COMMON_ROOT}/progress/SafeComplexProgress.cpp
					${COMMON_ROOT}/progress/SafeSimpleProgress.cpp
					${COMMON_ROOT}/service/messages/CowRequest.cpp
					${COMMON_ROOT}/service/messages/FarmOrganisation.cpp
					${COMMON_ROOT}/service/messages/FetchClassesResponse.cpp
					${COMMON_ROOT}/service/messages/FetchCowsResponse.cpp
					${COMMON_ROOT}/service/messages/FetchHentsResponse.cpp
					${COMMON_ROOT}/service/messages/FetchInvoicesResponse.cpp
					${COMMON_ROOT}/service/messages/FetchManifest.cpp
					${COMMON_ROOT}/service/messages/FetchStocksResponse.cpp
					${COMMON_ROOT}/service/messages/FetchPrintProfilesResponse.cpp
					${COMMON_ROOT}/service/messages/FetchPurchaseHeadersResponse.cpp
					${COMMON_ROOT}/service/messages/PrintProfileRequest.cpp
					${COMMON_ROOT}/service/messages/HentRequest.cpp
					${COMMON_ROOT}/service/messages/LoginRequest.cpp
					${COMMON_ROOT}/service/messages/LoginResponse.cpp
					${COMMON_ROOT}/service/messages/MarkInvoicePaidRequest.cpp
					${COMMON_ROOT}/service/messages/OpenSeasonResponse.cpp					
					${COMMON_ROOT}/service/messages/Settings.cpp
					${COMMON_ROOT}/service/messages/TransactionRequest.cpp
					${COMMON_ROOT}/service/messages/TransactionResponse.cpp
					${COMMON_ROOT}/service/messages/FetchDeductionDefinitionsResponse.cpp
					${COMMON_ROOT}/service/messages/PurchaseQuery.cpp
					${COMMON_ROOT}/service/messages/FetchPurchasesResponse.cpp
					${COMMON_ROOT}/service/messages/FetchPurchaseResponse.cpp
					${COMMON_ROOT}/service/messages/DeductionDefinitionRequest.cpp
					${COMMON_ROOT}/service/notification/Notification.cpp
					${COMMON_ROOT}/service/notification/NopNotificationRelay.cpp
					${COMMON_ROOT}/service/notification/NotificationEnvelope.cpp
					${COMMON_ROOT}/service/TypesRegistration.cpp
					${COMMON_ROOT}/service/wire/CowWireObject.cpp
					${COMMON_ROOT}/service/wire/HentWireObject.cpp
					${COMMON_ROOT}/ported/strptime.cpp
					${COMMON_ROOT}/string/IntString.cpp
					${COMMON_ROOT}/string/TextUtils.cpp
					${COMMON_ROOT}/types/DateRange.cpp
					${COMMON_ROOT}/types/DateTime.cpp
					${COMMON_ROOT}/types/Decimal.cpp
					${COMMON_ROOT}/types/GeoCoordinate.cpp
					${COMMON_ROOT}/types/Latitude.cpp
					${COMMON_ROOT}/types/Longitude.cpp
					${COMMON_ROOT}/types/NullDecimal.cpp
					${COMMON_ROOT}/types/NullDouble.cpp
					${COMMON_ROOT}/types/NullInt.cpp
					${COMMON_ROOT}/types/Time.cpp
					${COMMON_ROOT}/utils/iif.cpp
					${COMMON_ROOT}/utils/BoolUtil.cpp
					${COMMON_ROOT}/utils/WildcardMatch.cpp
					${COMMON_ROOT}/utils/JsonUtils.cpp
					${COMMON_ROOT}/utils/IdGenerator.cpp	
					${COMMON_ROOT}/string/Utf8.cpp
					${COMMON_ROOT}/string/ToString.cpp
					${COMMON_ROOT}/validation/CowNoValidator.cpp
					${COMMON_ROOT}/configuration/print/PrintProfiles.cpp
					${COMMON_ROOT}/configuration/print/PrintOption.cpp
					${COMMON_ROOT}/configuration/print/OrderSpecifier.cpp
					${COMMON_ROOT}/configuration/passportrecognition/PassportField.cpp
					${COMMON_ROOT}/configuration/passportrecognition/PassportFieldDescriptor.cpp
					${COMMON_ROOT}/configuration/passportrecognition/PassportFieldMetrics.cpp
					${COMMON_ROOT}/configuration/passportrecognition/PassportFields.cpp
					${COMMON_ROOT}/configuration/passportrecognition/PassportRecognitionProfile.cpp
					${COMMON_ROOT}/configuration/passportrecognition/PassportRecognitionProfiles.cpp
					${COMMON_ROOT}/configuration/serialization/SerializablePrintProfile.cpp
					${COMMON_ROOT}/configuration/deductions/DeductionDefinition.cpp
					${COMMON_ROOT}/configuration/deductions/DeductionDefinitions.cpp
					${COMMON_ROOT}/configuration/deductions/DeductionDetails.cpp
					${COMMON_ROOT}/configuration/serialization/SerializableDeductionDetails.cpp
					${COMMON_ROOT}/configuration/serialization/SerializableDeductionDefinition.cpp
					${COMMON_ROOT}/service/messages/FetchPassportRecognitionProfilesResponse.cpp
					${COMMON_ROOT}/service/messages/PassportRecognitionProfileRequest.cpp
					${COMMON_ROOT}/configuration/serialization/SerializablePassportRecognitionProfile.cpp
					)

IF(NOT BK_3RDPARTY_ROOT)
    SET(BK_3RDPARTY_ROOT ${CMAKE_SOURCE_DIR}/../../../bk3rdparty )
ENDIF()
					

IF(NOT BOOST_ROOT)
    SET(BOOST_ROOT /root/boost_1_67_0 )
ENDIF()

IF(NOT RCF_ROOT)
    SET(RCF_ROOT ${BK_3RDPARTY_ROOT}/RCF-2.2.0.0 )
ENDIF()

IF(NOT MPDECIMAL_ROOT)
    SET(MPDECIMAL_ROOT ${BK_3RDPARTY_ROOT}/mpdecimal-2.4.0 )
ENDIF()

IF(NOT BIN_PATH)
    SET(BIN_PATH ${CMAKE_SOURCE_DIR}/bin)
ENDIF()

IF(NOT UTF8CPP_ROOT)
    SET(UTF8CPP_ROOT ${BK_3RDPARTY_ROOT}/utf8cpp )
ENDIF()

IF(NOT JSONRPCCLIENT_ROOT)
    SET(JSONRPCCLIENT_ROOT ${BK_3RDPARTY_ROOT}/libjson-rpc-cpp )
ENDIF()

IF(NOT CASAJSON_ROOT)
    SET(CASAJSON_ROOT ${BK_3RDPARTY_ROOT}/casajson )
ENDIF()

IF(NOT ACTIVEMQCPP_ROOT)
    SET(ACTIVEMQCPP_ROOT ${BK_3RDPARTY_ROOT}/activemq-cpp-library-3.8.3 )
ENDIF()

IF(NOT CPPREST_ROOT)
    SET(CPPREST_ROOT ${BK_3RDPARTY_ROOT}/casablanca/Release )
ENDIF()

IF(NOT LOG4CXX_INCLUDE)
    SET(LOG4CXX_ROOT /usr/local/include/log4cxx )
ENDIF()



SET(EXECUTABLE_OUTPUT_PATH ${BIN_PATH} )
SET(LIBRARY_OUTPUT_PATH ${BIN_PATH})
LINK_DIRECTORIES( ${BIN_PATH} )

SET(
    RCF_DEFINES
    -DWIN32_LEAN_AND_MEAN
    -DBOOST_ALL_NO_LIB )

SET(
    RCF_INCLUDES     
    ${RCF_ROOT}/include)
	
SET(LIB_DIRS 
	${RCF_ROOT}/cmake/bin
	${MPDECIMAL_ROOT}/libmpdec
	/usr/lib/x86_64-linux-gnu	
	${JSONRPCCLIENT_ROOT}/cmake/bin
	${CASAJSON_ROOT}/cmake/bin
	/usr/local/lib
	${BOOST_ROOT}/stage/lib
	${BK_3RDPARTY_ROOT}/url-cpp/bin
	${BK_3RDPARTY_ROOT}/jwt-cpp/build/src
	)
    
IF(MINGW)
    SET(LIBS ws2_32 mswsock crypt32)
ELSEIF(MSVC)
    SET(LIBS ws2_32)
    IF(CMAKE_CL_64)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    ENDIF()
ELSE()
    SET(LIBS pthread dl boost_system boost_filesystem mpdec boost_thread boost_chrono pq boost_program_options jsonrpc casajson activemq-cpp boost_locale log4cxx ssl crypto urlcpp jwt )
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wno-deprecated-declarations")

SET(CMAKE_BUILD_TYPE Debug)

ADD_DEFINITIONS( ${RCF_DEFINES} -DUSE_WIDECHAR )

INCLUDE_DIRECTORIES( ${RCF_INCLUDES} ${SRC_ROOT} ${COMMON_ROOT} ${BOOST_ROOT} ${MPDECIMAL_ROOT}/libmpdec /usr/include/postgresql ${UTF8CPP_ROOT} ${JSONRPCCLIENT_ROOT}/src ${JSONCPP_ROOT}/include ${ACTIVEMQCPP_ROOT}/src/main ${CPPREST_ROOT}/include ${CASAJSON_ROOT}/include ${LOG4CXX_INCLUDE} ${BK_3RDPARTY_ROOT}/typestring ${BK_3RDPARTY_ROOT}/url-cpp/include  ${BK_3RDPARTY_ROOT}/jwt-cpp/src/include /usr/local/apr/include/apr-1 )

LINK_DIRECTORIES( ${LIB_DIRS} )

ADD_EXECUTABLE(BkServer ${COMMON_SRCS} ${SRCS})

TARGET_LINK_LIBRARIES( BkServer RcfLib ${LIBS} )
