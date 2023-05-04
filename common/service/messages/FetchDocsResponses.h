#pragma once 

#include "../wire/WireObjects.h"
#include "FetchDocsResponse.h"

typedef FetchDocsResponse<InDocWireObject> FetchInDocsResponse;
typedef boost::shared_ptr<FetchInDocsResponse> FetchInDocsResponseObj;

typedef FetchDocsResponse<BuyDocWireObject> FetchBuyDocsResponse;
typedef boost::shared_ptr<FetchBuyDocsResponse> FetchBuyDocsResponseObj;

typedef FetchDocsResponse<MoveDocWireObject> FetchMoveDocsResponse;
typedef boost::shared_ptr<FetchMoveDocsResponse> FetchMoveDocsResponseObj;

typedef FetchDocsResponse<OutDocWireObject> FetchOutDocsResponse;
typedef boost::shared_ptr<FetchOutDocsResponse> FetchOutDocsResponseObj;

typedef FetchDocsResponse<SellDocWireObject> FetchSellDocsResponse;
typedef boost::shared_ptr<FetchSellDocsResponse> FetchSellDocsResponseObj;

