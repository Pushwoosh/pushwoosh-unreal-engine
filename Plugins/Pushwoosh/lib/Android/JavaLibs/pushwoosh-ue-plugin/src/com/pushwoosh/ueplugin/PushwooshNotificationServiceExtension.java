package com.pushwoosh.ueplugin;

import com.pushwoosh.notification.NotificationServiceExtension;
import com.pushwoosh.notification.PushMessage;

public class PushwooshNotificationServiceExtension extends NotificationServiceExtension {
	@Override
	protected boolean onMessageReceived(final PushMessage pushMessage) {
		PushwooshPlugin.handlePushReceived(pushMessage.toJson().toString());
		return (PushwooshPlugin.getInstance() != null && !PushwooshPlugin.getInstance().isShowPushInForeground() && isAppOnForeground()) || super.onMessageReceived(pushMessage);
	}

	@Override
	protected void startActivityForPushMessage(final PushMessage pushMessage) {
		super.startActivityForPushMessage(pushMessage);
		PushwooshPlugin.handlePushOpen(pushMessage.toJson().toString());
	}
}
