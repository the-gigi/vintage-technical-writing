using System;

namespace R
{
	public interface IObjectProvider
	{
		SerialObject	SerialObject { get; }
		IRemoteObject	RemoteObject { get; }
	}
}
