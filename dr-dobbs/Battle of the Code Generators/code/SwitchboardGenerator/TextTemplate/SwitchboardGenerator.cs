using System;
using System.Collections;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Text;

namespace TextTemplate
{
	/// <summary>
	/// Generates a Switchboard class from a list of interface names
	/// </summary>
	public class SwitchboardGenerator
	{
		public static bool Generate(string nameSpace, string filename, ArrayList interfaces) 
		{
			if (nameSpace.Length == 0 || interfaces.Count == 0)
			{
				Debug.Assert(false);
				return false;
			}

			string code = CreateSwitchboard(nameSpace, interfaces);
			Console.WriteLine(code);
			StreamWriter writer = new StreamWriter (new FileStream (filename, FileMode.Create));
			writer.Write(code);
			writer.Close();
			
			return true;
		}		

	
		static private string GetListName(Type i)
		{
			return string.Format("m_{0}{1}List", 
				i.Name.Substring(1,1).ToLower(),
				i.Name.Substring(2, i.Name.Length-2));
		}

		static private string CreateEvent(MethodInfo mi, string comment)
		{
			//		$$$-comment-$$$
			//		public $$$-return-type-$$$ $$$-event_name-$$$($$$-parameter_defs-$$$)
			//		{
			//			foreach ($$$-Interface-$$$ subscriber in $$$-sink_list-$$$)
			//				subscriber.$$$-event-$$$($$$-parameter_list-$$$);
			//		}
			ParameterInfo[] parameters = mi.GetParameters();
			// Prepare parameter definitions and list
			StringBuilder parameterDefs = new StringBuilder();
			StringBuilder parameterList = new StringBuilder();
			if (parameters.Length > 0)
			{
				foreach (ParameterInfo pi in parameters)
				{
					parameterDefs.AppendFormat("{0} {1},", pi.ParameterType.FullName, pi.Name);
					parameterList.AppendFormat("{0},", pi.Name);
				}
				// remove last redundant comma
				parameterDefs.Remove(parameterDefs.Length-1,1);
				parameterList.Remove(parameterList.Length-1,1);
			}

			StringBuilder result = new StringBuilder(TemplateProvider.GetEventTemplate());
			result.Replace("$$$-comment-$$$", comment);
			result.Replace("$$$-return_type-$$$", mi.ReturnType.FullName);
			result.Replace("$$$-event-$$$", mi.Name);
			result.Replace("$$$-parameter_defs-$$$", parameterDefs.ToString());
			result.Replace("$$$-parameter_list-$$$", parameterList.ToString());
			result.Replace("$$$-Interface-$$$", mi.DeclaringType.FullName);
			result.Replace("$$$-sink_list-$$$", GetListName(mi.DeclaringType));

			return result.ToString();
		}


		static private string CreateSubscribeUnsubscribeMethods(Type i)
		{
//			public bool Subscribe($$$-Interface-$$$ sink)
//			{
//				if ($$$-sink_list-$$$.Contains(sink))
//				{
//					Debug.Assert(false);
//					  return false;
//				}
//				$$$-sink_list-$$$.Add(sink);
//				return true;
//			}
//
//			public bool Unsubscribe($$$-Interface-$$$ sink)
//			{
//				if (!$$$-sink_list-$$$.Contains(sink))
//				{
//					Debug.Assert(false);
//					return false;
//				}
//				$$$-sink_list-$$$.Add(sink);
//				return true;
//			}

			string template = TemplateProvider.GetSubscribeUnsubscribeTemplate();
			StringBuilder result = new StringBuilder(template);
			result.Replace("$$$-Interface-$$$", i.FullName);
			result.Replace("$$$-sink_list-$$$", GetListName(i));
			return result.ToString();
		}

		static private string CreateInterface(Type i)
		{
			// Opening comment
			string comment = String.Format ("{0} events", i.Name);
			ArrayList members = new ArrayList(i.GetMembers()); 
			members.Reverse();
			StringBuilder result = new StringBuilder();
			foreach (MethodInfo mi in members)
			{
				string c = "";
				if (members[0] == mi)
					c = comment;
				
				result.Append(CreateEvent(mi, null));
			}

			return result.ToString();
		}

		static private string CreateSwitchboard(string nameSpace, ArrayList interfaces)
		{
			//// Auto-generated Switchboard class
			//namespace $$$-namespace-$$$
			//{
			//		class Switchboard :
			//$$$-base_interfaces-$$$
			//		{
			//$$$-subscribe_unsubscribe_methods-$$$
			//$$$-event_methods-$$$
			//
			//$$$-sink_lists-$$$ 
			//		}
			//	}

			Debug.Assert(nameSpace != null && interfaces.Count > 0);

			string template = TemplateProvider.GetSwitchboardTemplate();
			StringBuilder result = new StringBuilder(template);
			result.Replace("$$$-namespace-$$$", nameSpace);

			// Add base interfaces
			StringBuilder baseInterfaces = new StringBuilder();
			foreach (Type i in interfaces)
				baseInterfaces.Append(string.Format("\t\t{0},\n", i.FullName));
			baseInterfaces.Remove(baseInterfaces.Length-2,2);

			result.Replace("$$$-base_interfaces-$$$", baseInterfaces.ToString());

			// Add subscribe/unsubscribe methods
			StringBuilder subscribeUnsubscribeMethods = new StringBuilder();
			foreach (Type i in interfaces)
				subscribeUnsubscribeMethods.Append(CreateSubscribeUnsubscribeMethods(i));
			result.Replace("$$$-subscribe_unsubscribe_methods-$$$", 
				subscribeUnsubscribeMethods.ToString());

			// Add all interface event methods
			StringBuilder eventMethods = new StringBuilder();
			foreach (Type i in interfaces)
				eventMethods.Append(CreateInterface(i));
			result.Replace("$$$-event_methods-$$$", eventMethods.ToString());
			Console.WriteLine(result.ToString());

			// Add sink lists
			StringBuilder sinksLists = new StringBuilder();
			foreach (Type i in interfaces)
				sinksLists.AppendFormat("\t\tArrayList {0} = new ArrayList();\n", 
					GetListName(i));
			
			result.Replace("$$$-sink_lists-$$$", sinksLists.ToString());

			// Globaly replace all System.Void with plain 'void'
			result.Replace("System.Void", "void");

			return result.ToString();
		}
	}  	
}
