using System;

namespace R
{
	class ObjectProvider : MarshalByRefObject, IObjectProvider
	{
		ObjectProvider()
		{
			Console.WriteLine("ObjectProvider Created!!!");			
		}

		public IRemoteObject RemoteObject
		{
			get { return m_remote; }
		}
		public SerialObject SerialObject
		{
			get { return m_serialized; }
		}


		RemoteObject	m_remote = new RemoteObject();
		SerialObject	m_serialized = new SerialObject();
	}
}
