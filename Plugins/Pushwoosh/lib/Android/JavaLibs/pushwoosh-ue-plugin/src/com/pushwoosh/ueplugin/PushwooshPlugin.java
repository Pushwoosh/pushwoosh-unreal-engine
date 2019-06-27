package com.pushwoosh.ueplugin;

import java.util.Map;
import java.util.concurrent.atomic.AtomicBoolean;

import org.json.JSONException;
import org.json.JSONObject;

import com.pushwoosh.Pushwoosh;
import com.pushwoosh.exception.RegisterForPushNotificationsException;
import com.pushwoosh.function.Result;
import com.pushwoosh.inapp.PushwooshInApp;
import com.pushwoosh.internal.utils.PWLog;
import com.pushwoosh.tags.TagsBundle;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

public class PushwooshPlugin
{
	public static final String TAG = "PushwooshUEPlugin";
	
	private static PushwooshPlugin mInstance;
	private static String mStartPushData;
	private static String mStartReceivedPushData;
	private boolean showInForeground;
	
	private static native void native_onPushRegistered(String token);
	private static native void native_onPushRegistrationError(String message);
	private static native void native_onPushAccepted(String payload);
	private static native void native_onPushReceived(String payload);
	
	public static PushwooshPlugin getInstance() {
		return mInstance;
	}
	
	public PushwooshPlugin(Activity activity) {
		mInstance = this;
	}
	
	public static void handlePushOpen(String pushPayload) {
		if (PushwooshPlugin.getInstance() == null) {
			synchronized(TAG) {
				mStartPushData = pushPayload;
			}
		} else {
			native_onPushAccepted(pushPayload);
		}
	}
		
	public static void handlePushReceived(String pushPayload) {
		if (PushwooshPlugin.getInstance() == null) {
			synchronized(TAG) {
				mStartReceivedPushData = pushPayload;
			}
		} else {
			native_onPushReceived(pushPayload);
		}
	}

	public boolean isShowPushInForeground() {
		return showInForeground;
	}
	/// External plugin methods
	
	public void initialize(String appId, String fcmProject, boolean showInForeground) {
		PWLog.debug(TAG, "initialize(" + appId + ", " + fcmProject + ") method called ");
		try {
			Pushwoosh.getInstance().setAppId(appId);
			Pushwoosh.getInstance().setSenderId(fcmProject);
			this.showInForeground = showInForeground;

			synchronized(TAG) {
				if (mStartReceivedPushData != null) {
					native_onPushReceived(mStartPushData);
					mStartReceivedPushData = null;
				}
				if (mStartPushData != null) {
					native_onPushAccepted(mStartPushData);
					mStartPushData = null;
				}
			}
		} 
		catch (Exception e) {
			PWLog.exception(e);
		}
	}
	
	public void registerForPushNotifications() {
		Pushwoosh.getInstance().registerForPushNotifications(new com.pushwoosh.function.Callback<String, RegisterForPushNotificationsException>() {
			@Override
			public void process(Result<String, RegisterForPushNotificationsException> result) {
				if (result.isSuccess()) {
					native_onPushRegistered(result.getData());
				} else if (result.getException() != null) {
					native_onPushRegistrationError(result.getException().getLocalizedMessage());
				}
			}
		});
	}
	
	public void unregisterForPushNotifications() {
		PWLog.debug(TAG, "unregisterForPushNotifications() method called ");
		
		Pushwoosh.getInstance().unregisterForPushNotifications();
	}
	
	public void setTags(String tagsStr) {
		PWLog.debug(TAG, "setTags(\"" + tagsStr +  "\") method called ");
		try {
			JSONObject json = new JSONObject(tagsStr);
			
			Pushwoosh.getInstance().sendTags(new TagsBundle.Builder().putAll(json).build());
		}
		catch(JSONException e) {
			PWLog.exception(e);
		}
	}
	
	public void setUserId(String userId) {
		PWLog.debug(TAG, "setUserId(\"" + userId +  "\") method called ");
		
		PushwooshInApp.getInstance().setUserId(userId);
	}
	
	public void postEvent(String event, String attributes) {
		PWLog.debug(TAG, "postEvent(\"" + event +  "\", " + attributes + ") method called ");
		
		attributes = attributes.trim();
		if (attributes.isEmpty()) {
			attributes = "{}";
		}
		
		try {
			PushwooshInApp.getInstance().postEvent(event, new TagsBundle.Builder().putAll(new JSONObject(attributes)).build());
		}
		catch (JSONException e) {
			PWLog.exception(e);
		}
	}
}