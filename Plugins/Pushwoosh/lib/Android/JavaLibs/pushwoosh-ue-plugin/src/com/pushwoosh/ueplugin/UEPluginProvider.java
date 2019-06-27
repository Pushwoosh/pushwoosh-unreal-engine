package com.pushwoosh.ueplugin;

import com.pushwoosh.internal.PluginProvider;

public class UEPluginProvider implements PluginProvider {
	@Override
	public String getPluginType() {
		return "Unreal Engine";
	}

	@Override
	public int richMediaStartDelay() {
		return DEFAULT_RICH_MEDIA_START_DELAY;
	}
}
