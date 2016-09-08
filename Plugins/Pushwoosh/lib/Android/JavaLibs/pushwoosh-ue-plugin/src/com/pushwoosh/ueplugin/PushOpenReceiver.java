package com.pushwoosh.ueplugin;

import org.json.JSONObject;

import com.pushwoosh.internal.PushManagerImpl;
import com.pushwoosh.internal.utils.PWLog;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

public class PushOpenReceiver extends BroadcastReceiver
{
    public void onReceive(Context context, Intent intent)
    {
        if (intent == null)
            return;

        PWLog.debug(PushwooshPlugin.TAG, "Push notification opened");
        
        Bundle pushBundle = PushManagerImpl.preHandlePush(context, intent);
        if(pushBundle == null)
            return;
             
        JSONObject dataObject = PushManagerImpl.bundleToJSON(pushBundle);
         
        startActivity(context);
         
        PushManagerImpl.postHandlePush(context, intent);
        
        PushwooshPlugin.handlePushOpen(dataObject.toString());
    }
    
    private void startActivity(Context context)
    {
    	if (PushwooshPlugin.getInstance() != null && PushwooshPlugin.getInstance().isGameActivityAlive())
    	{
    		PWLog.debug(PushwooshPlugin.TAG, "Starting GameActivity...");
    		
    		// Game activity is already started. Bring it to front.
    		try
			{
				Intent launchIntent = new Intent(context, Class.forName("com.epicgames.ue4.GameActivity"));
				launchIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
				context.startActivity(launchIntent);
				return;
			}
			catch (ClassNotFoundException e)
			{
				PWLog.exception(e);
			}
    	}
    	
    	PWLog.debug(PushwooshPlugin.TAG, "Starting default launcher activity...");
    	
		// Game activity is not started yet. Need to start it through default LAUNCHER activity.
        Intent launchIntent  = context.getPackageManager().getLaunchIntentForPackage(context.getPackageName());
        launchIntent.addCategory("android.intent.category.LAUNCHER");
        launchIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
        
        context.startActivity(launchIntent);
    }
}
