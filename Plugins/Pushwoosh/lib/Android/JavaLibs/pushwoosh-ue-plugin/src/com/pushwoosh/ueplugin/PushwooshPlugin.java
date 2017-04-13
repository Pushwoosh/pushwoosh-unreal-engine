package com.pushwoosh.ueplugin;

import java.util.Map;
import java.util.concurrent.atomic.AtomicBoolean;

import org.json.JSONException;
import org.json.JSONObject;

import com.pushwoosh.BasePushMessageReceiver;
import com.pushwoosh.BaseRegistrationReceiver;
import com.pushwoosh.PushManager;
import com.pushwoosh.inapp.InAppFacade;
import com.pushwoosh.internal.utils.JsonUtils;
import com.pushwoosh.internal.utils.PWLog;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

public class PushwooshPlugin
{
	public static final String TAG = "PushwooshUEPlugin";
	
	private final Context mContext;
	private final Activity mGameActivity;
	private boolean mGameActivityAlive = false;
	private PushManager mPushManager;
	private AtomicBoolean mRegistering = new AtomicBoolean(false);
	
	private static PushwooshPlugin mInstance;
	private static String mStartPushData;
	
	private static native void native_onPushRegistered(String token);
	private static native void native_onPushRegistrationError(String message);
	private static native void native_onPushAccepted(String payload);
	
	public static PushwooshPlugin getInstance()
	{
		return mInstance;
	}
	
	public PushwooshPlugin(Activity activity)
	{
		mInstance = this;
		mGameActivity = activity;
		mContext = activity.getApplicationContext();
	}
	
	public boolean isGameActivityAlive()
	{
		return mGameActivityAlive;
	}
	
	public static void handlePushOpen(String pushPayload)
	{
		if (PushwooshPlugin.getInstance() == null)
		{
			synchronized(TAG)
			{
				// postpone push accepted callback until application will be ready to handle it
				mStartPushData = pushPayload;
			}
		}
		else
		{
			native_onPushAccepted(pushPayload);
		}
	}
	
	/// External plugin methods
	
	public void initialize(String appId, String gcmProject)
	{
		PWLog.debug(TAG, "initialize(" + appId + ", " + gcmProject + ") method called ");
		
		try 
		{
			PushManager.initializePushManager(mContext, appId, gcmProject);
			mPushManager = PushManager.getInstance(mContext);
			mPushManager.onStartup(mContext);
			
			synchronized(TAG)
			{
				if (mStartPushData != null)
				{
					native_onPushAccepted(mStartPushData);
					mStartPushData = null;
				}
			}
		} 
		catch (Exception e) 
		{
			PWLog.exception(e);
		}
	}
	
	public void registerForPushNotifications()
	{
		mRegistering.set(true);
		
		PWLog.debug(TAG, "registerForPushNotifications() method called ");
		
		mPushManager.registerForPushNotifications();
	}
	
	public void unregisterForPushNotifications()
	{
		PWLog.debug(TAG, "unregisterForPushNotifications() method called ");
		
		mPushManager.unregisterForPushNotifications();
	}
	
	public void setTags(String tagsStr)
	{
		PWLog.debug(TAG, "setTags(\"" + tagsStr +  "\") method called ");
		
		try
		{
			JSONObject json = new JSONObject(tagsStr);
			Map<String, Object> tags = JsonUtils.jsonToMap(json);
			
			PushManager.sendTags(mContext, tags, null);
		}
		catch(JSONException e)
		{
			PWLog.exception(e);
		}
	}
	
	public void setUserId(String userId)
	{
		PWLog.debug(TAG, "setUserId(\"" + userId +  "\") method called ");
		
		PushManager.getInstance(mContext).setUserId(mContext, userId);
	}
	
	public void postEvent(String event, String attributes)
	{
		PWLog.debug(TAG, "postEvent(\"" + event +  "\", " + attributes + ") method called ");
		
		attributes = attributes.trim();
		if (attributes.isEmpty()) 
		{
			attributes = "{}";
		}
		
		try
		{
			Map<String, Object> attributesMap = JsonUtils.jsonToMap(new JSONObject(attributes));
			InAppFacade.postEvent(mGameActivity, event, attributesMap);
		}
		catch (JSONException e)
		{
			PWLog.exception(e);
		}
	}
	
	/// Activity lifecycle callbacks
	
	public void onGameActivityCreate()
	{
		PWLog.debug(TAG, "GameActivity created");
		
		mRegistrationReceiver.register(mContext);
		mPushReceiver.register(mContext);
		
		mGameActivityAlive = true;
	}
	
	public void onGameActivityDestroy()
	{
		PWLog.debug(TAG, "GameActivity destroyed");
		
		mRegistrationReceiver.unregister(mContext);
		
		mGameActivityAlive = false;
	}
	
	public void onGameActivityPause()
	{
		PWLog.debug(TAG, "GameActivity paused");
		
		mPushReceiver.unregister(mContext);
	}
	
	public void onGameActivityResume()
	{
		PWLog.debug(TAG, "GameActivity resumed");
		
		mPushReceiver.register(mContext);
	}
	
	/// Private 
	
	private static class PushMessageReceiver extends BasePushMessageReceiver
	{
		@Override
		protected void onMessageReceive(Intent intent)
		{
			PWLog.debug(TAG, "Push notification opened in foreground");
			
			native_onPushAccepted(intent.getStringExtra(JSON_DATA_KEY));
		}
		
		public void register(Context context)
		{
			unregister(context);
			
			IntentFilter intentFilter = new IntentFilter(context.getPackageName() + ".action.PUSH_MESSAGE_RECEIVE");
			context.registerReceiver(this, intentFilter);
		}
		
		public void unregister(Context context)
		{
			try
			{
				context.unregisterReceiver(this);
			}
			catch (Exception e)
			{
				// pass.
			}
		}
	}
	
	private PushMessageReceiver mPushReceiver = new PushMessageReceiver();
	
	
	private class RegistrationReceiver extends BaseRegistrationReceiver
	{
		@Override
		public void onRegisterActionReceive(Context context, Intent intent)
		{
			if (null != intent && mRegistering.getAndSet(false))
			{
				if (intent.hasExtra(PushManager.REGISTER_EVENT))
				{
					native_onPushRegistered(intent.getExtras().getString(PushManager.REGISTER_EVENT));
				}
				else if (intent.hasExtra(PushManager.REGISTER_ERROR_EVENT))
				{
					native_onPushRegistrationError(intent.getExtras().getString(PushManager.REGISTER_ERROR_EVENT));
				}
			}
		}
		
		public void register(Context context)
		{
			unregister(context);
			
			IntentFilter intentFilter = new IntentFilter(context.getPackageName() + "." + PushManager.REGISTER_BROAD_CAST_ACTION);
			context.registerReceiver(this, intentFilter);	
		}
		
		public void unregister(Context context)
		{
			try
			{
				context.unregisterReceiver(this);
			}
			catch (Exception e)
			{
				// pass.
			}
		}
	}
	
	private RegistrationReceiver mRegistrationReceiver = new RegistrationReceiver();
}
