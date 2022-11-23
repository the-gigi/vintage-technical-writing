using System;
using System.Windows.Forms;

namespace Cosurfer
{  
	class Factory
	{
		[STAThread]
		public static void Main(string[] args) 
		{
			ConnectionManager connectionManager = new ConnectionManager();
			CosurfEngine engine = new CosurfEngine(connectionManager);
			MainForm mainForm = new MainForm(engine);

			connectionManager.AttachSink(engine);
			engine.AttachSink(mainForm);

			connectionManager.Listen();

			Application.Run(mainForm);
		}
	}   
}
