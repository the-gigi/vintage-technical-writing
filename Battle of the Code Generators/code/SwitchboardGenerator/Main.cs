using System;
using System.Collections;
using System.Reflection;
using System.IO;

namespace Gen
{
	class MainClass
	{
		[STAThread]
			/// <summary>
			/// Generates a Switchboard class.
			/// 
			/// output_path - the full path of the generated Switchboard class
			/// namespace	- the namespace of the generated Switchboard class
			/// attribute	- the custom attribute type name that identifies event interfaces
			/// assemblies	- the path to the assemblies that need to be scanned and contain
			///				  event interfaces.
			/// </summary>
		static void Main(string[] args)
		{
			if (args.Length < 5)
			{
				string u = "Usage: TextTemplate <output_path> <namespace> <attribute> <generator> <assembly> [<assembly> ...]";
				Console.WriteLine(u);
				return;
			}

			string filename		= args[0];
			string ns			= args[1];
			string attribute	= args[2];		
			string generator	= args[3]; // CodeDOM or TextTemplate

			ArrayList interfaces = new ArrayList();

			for (int i = 4; i < args.Length; ++i)
			{
				// Load assembly
				string path = Path.GetFullPath(args[i]);
				Console.WriteLine(path);
				Assembly a = Assembly.LoadFile(path);

				// Extract assembly types
				Type[] types = a.GetTypes();
				
				// iterate over types and probe custom attributes of each type
				foreach (Type t in types)
				{
					ArrayList oa = new ArrayList(t.GetCustomAttributes(false));
					if (oa.Count > 0)
					{
						Console.WriteLine("The Attribute is : {0}", oa[0].ToString());
						if (oa[0].ToString() == attribute)
						{
							Console.WriteLine("Discovered {0}", t.FullName);

							// Store interfaces that carry the event attribute
							interfaces.Add(t);
						}
					}
				}
			}
			
			// Launch dynamically the selected code generator (CodeDOM.SwitchboardGenerator or TextTemplate.SwitchboardGenerator)
			Type sw = Type.GetType(String.Format("{0}.SwitchboardGenerator", generator));
			BindingFlags flags = BindingFlags.Public|BindingFlags.Static|BindingFlags.InvokeMethod;
			try
			{
				bool b = (bool)sw.InvokeMember("Generate", flags, null, null, new object[] { ns, filename, interfaces });
			}
			catch (Exception e)
			{
				Console.WriteLine("{0} {1}", e.TargetSite , e.Message);
			}
		}
	}
}
