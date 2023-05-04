#pragma once

#include "DeleteNotification.h"

struct tag_CowDeleteNotification {};
typedef DeleteNotification<tag_CowDeleteNotification,&NotificationRelay::CowDeleted> CowDeleteNotification;

struct tag_HentDeleteNotification {};
typedef DeleteNotification<tag_HentDeleteNotification, &NotificationRelay::HentDeleted> HentDeleteNotification;

struct tag_InDocDeleteNotification {};
typedef DeleteNotification<tag_InDocDeleteNotification, &NotificationRelay::InDocDeleted> InDocDeleteNotification;

struct tag_BuyDocDeleteNotification {};
typedef DeleteNotification<tag_BuyDocDeleteNotification, &NotificationRelay::BuyDocDeleted> BuyDocDeleteNotification;

struct tag_MoveDocDeleteNotification {};
typedef DeleteNotification<tag_MoveDocDeleteNotification, &NotificationRelay::MoveDocDeleted> MoveDocDeleteNotification;

struct tag_SellDocDeleteNotification {};
typedef DeleteNotification<tag_SellDocDeleteNotification, &NotificationRelay::SellDocDeleted> SellDocDeleteNotification;

struct tag_OutDocDeleteNotification {};
typedef DeleteNotification<tag_OutDocDeleteNotification, &NotificationRelay::OutDocDeleted> OutDocDeleteNotification;

struct tag_BuyInvoiceDeleteNotification {};
typedef DeleteNotification<tag_BuyInvoiceDeleteNotification, &NotificationRelay::BuyInvoiceDeleted> BuyInvoiceDeleteNotification;

struct tag_SellInvoiceDeleteNotification {};
typedef DeleteNotification<tag_SellInvoiceDeleteNotification, &NotificationRelay::SellInvoiceDeleted> SellInvoiceDeleteNotification;

