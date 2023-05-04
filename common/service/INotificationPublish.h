#pragma once

#include <RCF/RCF.hpp>
#include "notification/Notification.h"
#include "notification/NotificationEnvelope.h"

typedef boost::shared_ptr<Notification> NotificationObj;

RCF_BEGIN(I_NotificationPublish, "I_NotificationPublish")
    RCF_METHOD_V1(void, Notify, NotificationObj)	
RCF_END(I_NotificationPublish)
