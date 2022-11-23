using System;
using System.Collections;

namespace DomainMasterLib
{
	public enum State
	{
		NONE,
		IDLE,
		RUNNING,
		SHUTTING_DOWN
	};

	public interface IDomainMasterEvents
	{
		void OnAssemblyLoad(string app, string assembly);
		void OnDomainUnload(string app);
	};
	
	public interface IAppInfo
	{
		string Path { get; }
		AppDomain Domain 
		{ 
			get;
			set;
		}

		State State 
		{ 
			get;
			set;
		}		
	};

	public interface IDomainMaster
	{
		void AttachSink(IDomainMasterEvents sink);
		bool AddApp(string app, string path);
		void Launch(string app);
		void Terminate(string app);
		ICollection Apps { get; }
	}
}
