using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;
using System.Diagnostics;
using System.Reflection;
using System.Text;
using DomainMasterLib;

namespace Launcher
{
	public class MainForm : System.Windows.Forms.Form, IDomainMasterEvents
	{
		private System.Windows.Forms.TabPage Apps_TabPAge;
		private System.Windows.Forms.TabPage Events_TabPage;
		private System.Windows.Forms.ListView Events_ListView;
		private System.Windows.Forms.Button New_Button;
		private System.Windows.Forms.Button Run_Button;
		private System.Windows.Forms.Button Stop_Button;
		private System.Windows.Forms.ListView Apps_ListView;
		private System.Windows.Forms.TabControl tabControl;
		private System.ComponentModel.Container components = null;

		public MainForm()
		{
			InitializeComponent();

			IDomainMaster dm = DomainMaster.Instance;

		}

		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.tabControl = new System.Windows.Forms.TabControl();
			this.Apps_TabPAge = new System.Windows.Forms.TabPage();
			this.Apps_ListView = new System.Windows.Forms.ListView();
			this.Stop_Button = new System.Windows.Forms.Button();
			this.Run_Button = new System.Windows.Forms.Button();
			this.New_Button = new System.Windows.Forms.Button();
			this.Events_TabPage = new System.Windows.Forms.TabPage();
			this.Events_ListView = new System.Windows.Forms.ListView();
			this.tabControl.SuspendLayout();
			this.Apps_TabPAge.SuspendLayout();
			this.Events_TabPage.SuspendLayout();
			this.SuspendLayout();
			// 
			// tabControl
			// 
			this.tabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.tabControl.Controls.Add(this.Apps_TabPAge);
			this.tabControl.Controls.Add(this.Events_TabPage);
			this.tabControl.Location = new System.Drawing.Point(0, 8);
			this.tabControl.Name = "tabControl";
			this.tabControl.SelectedIndex = 0;
			this.tabControl.Size = new System.Drawing.Size(400, 336);
			this.tabControl.TabIndex = 1;
			// 
			// Apps_TabPAge
			// 
			this.Apps_TabPAge.Controls.Add(this.Apps_ListView);
			this.Apps_TabPAge.Controls.Add(this.Stop_Button);
			this.Apps_TabPAge.Controls.Add(this.Run_Button);
			this.Apps_TabPAge.Controls.Add(this.New_Button);
			this.Apps_TabPAge.Location = new System.Drawing.Point(4, 22);
			this.Apps_TabPAge.Name = "Apps_TabPAge";
			this.Apps_TabPAge.Size = new System.Drawing.Size(392, 310);
			this.Apps_TabPAge.TabIndex = 0;
			this.Apps_TabPAge.Text = "Apps";
			// 
			// Apps_ListView
			// 
			this.Apps_ListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.Apps_ListView.FullRowSelect = true;
			this.Apps_ListView.HideSelection = false;
			this.Apps_ListView.Location = new System.Drawing.Point(0, 0);
			this.Apps_ListView.MultiSelect = false;
			this.Apps_ListView.Name = "Apps_ListView";
			this.Apps_ListView.Size = new System.Drawing.Size(392, 272);
			this.Apps_ListView.TabIndex = 3;
			this.Apps_ListView.View = System.Windows.Forms.View.List;
			this.Apps_ListView.SelectedIndexChanged += new System.EventHandler(this.Apps_ListView_SelectedIndexChanged);
			// 
			// Stop_Button
			// 
			this.Stop_Button.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.Stop_Button.Enabled = false;
			this.Stop_Button.Location = new System.Drawing.Point(168, 280);
			this.Stop_Button.Name = "Stop_Button";
			this.Stop_Button.TabIndex = 2;
			this.Stop_Button.Text = "Stop";
			this.Stop_Button.Click += new System.EventHandler(this.Stop_Button_Click);
			// 
			// Run_Button
			// 
			this.Run_Button.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.Run_Button.Enabled = false;
			this.Run_Button.Location = new System.Drawing.Point(88, 280);
			this.Run_Button.Name = "Run_Button";
			this.Run_Button.TabIndex = 1;
			this.Run_Button.Text = "Run";
			this.Run_Button.Click += new System.EventHandler(this.Run_Button_Click);
			// 
			// New_Button
			// 
			this.New_Button.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.New_Button.Location = new System.Drawing.Point(8, 280);
			this.New_Button.Name = "New_Button";
			this.New_Button.TabIndex = 0;
			this.New_Button.Text = "New...";
			this.New_Button.Click += new System.EventHandler(this.New_Button_Click);
			// 
			// Events_TabPage
			// 
			this.Events_TabPage.Controls.Add(this.Events_ListView);
			this.Events_TabPage.Location = new System.Drawing.Point(4, 22);
			this.Events_TabPage.Name = "Events_TabPage";
			this.Events_TabPage.Size = new System.Drawing.Size(392, 310);
			this.Events_TabPage.TabIndex = 1;
			this.Events_TabPage.Text = "Events";
			// 
			// Events_ListView
			// 
			this.Events_ListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.Events_ListView.FullRowSelect = true;
			this.Events_ListView.Location = new System.Drawing.Point(0, 0);
			this.Events_ListView.MultiSelect = false;
			this.Events_ListView.Name = "Events_ListView";
			this.Events_ListView.Size = new System.Drawing.Size(392, 304);
			this.Events_ListView.TabIndex = 0;
			this.Events_ListView.View = System.Windows.Forms.View.List;
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(400, 341);
			this.Controls.Add(this.tabControl);
			this.Name = "MainForm";
			this.Text = "Launcher";
			this.Load += new System.EventHandler(this.MainForm_Load);
			this.Closed += new System.EventHandler(this.MainForm_Closed);
			this.Paint += new System.Windows.Forms.PaintEventHandler(this.MainForm_Paint);
			this.tabControl.ResumeLayout(false);
			this.Apps_TabPAge.ResumeLayout(false);
			this.Events_TabPage.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void Run_Button_Click(object sender, System.EventArgs e)
		{
			Debug.Assert(Apps_ListView.SelectedItems.Count > 0);
			string app = Apps_ListView.SelectedItems[0].Text;
			DomainMaster.Instance.Launch(app);
			UpdateButtons();
		}

		private void Stop_Button_Click(object sender, System.EventArgs e)
		{
			Debug.Assert(Apps_ListView.SelectedItems.Count > 0);
			string app = Apps_ListView.SelectedItems[0].Text;
			DomainMaster.Instance.Terminate(app);
			UpdateButtons();
		}

		private void AddApp(string path)
		{
			string app = Path.GetFileNameWithoutExtension(path);
			bool rc = DomainMaster.Instance.AddApp(app, path);
			if (rc)
				Apps_ListView.Items.Add(app);
		}

		private void New_Button_Click(object sender, System.EventArgs e)
		{
			OpenFileDialog ofd = new OpenFileDialog();

			ofd.InitialDirectory = Directory.GetCurrentDirectory();
			ofd.Filter = "exe files (*.exe)|*.exe" ;
			ofd.FilterIndex = 1;
			ofd.RestoreDirectory = false;
			ofd.Multiselect = true;

			if (ofd.ShowDialog() != DialogResult.OK)
				return;

			foreach (string filename in ofd.FileNames)
				AddApp(filename);
		}

		private void MainForm_Load(object sender, System.EventArgs e)
		{
			m_appsPath		= Path.Combine(Directory.GetCurrentDirectory(), m_appsFile);			
			// Check if apps file exists
			if (!File.Exists(m_appsPath))
				return;
			try 
			{
				// Create an instance of StreamReader to read the apps file.
				// The using statement also closes the StreamReader.
				using (StreamReader sr = new StreamReader(m_appsPath)) 
				{
					string filename;
					while ((filename = sr.ReadLine()) != null) 
					{
						if (File.Exists(filename))
							AddApp(filename);
					}
				}
			}
			catch (Exception ex) 
			{
				// Let the user know what went wrong.
				Console.WriteLine("Unable to read from {0}", m_appsPath);
				Console.WriteLine(ex.Message);
			}
			finally
			{
				DomainMaster.Instance.AttachSink(this);
			}

		}

		private void MainForm_Closed(object sender, System.EventArgs e)
		{
			try 
			{
				// Store current app list in the file
				using (StreamWriter sw = new StreamWriter(m_appsPath)) 
				{
					foreach (IAppInfo app in DomainMaster.Instance.Apps)
						sw.WriteLine(app.Path);
				}
			}
			catch (Exception ex) 
			{
				// Let the user know what went wrong.
				Console.WriteLine("Unable to write to {0}", m_appsPath);
				Console.WriteLine(ex.Message);
			}				
		}

		private State GetState(string app)
		{
			foreach (IAppInfo ai in DomainMaster.Instance.Apps)
				if (ai.Domain != null && ai.Domain.FriendlyName == app)
					return ai.State;

			return State.IDLE;
		}

		private void UpdateButtons()
		{
			if (Apps_ListView.SelectedItems.Count == 0)
			{
				Run_Button.Enabled	= false;
				Stop_Button.Enabled = false;
			}
			else
			{
				string app = Apps_ListView.SelectedItems[0].Text;
				State state = GetState(app);
				Run_Button.Enabled =	state == State.IDLE;
				Stop_Button.Enabled =	state == State.RUNNING;
			}
		}

		private void Apps_ListView_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			UpdateButtons();
		}

		private void MainForm_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
		{
		}

		// IDomainMasterEvents
		public void OnDomainUnload(string app)
		{
			String item = String.Format("AppDomain Unloaded: '{0}'", app);
			Events_ListView.Items.Add(item);
			UpdateButtons();			
		}

		public void OnAssemblyLoad(string app, string assembly)
		{
			String item = String.Format("Assembly Loaded: '{0}' in domain '{1}'", assembly, app);
			Events_ListView.Items.Add(item);
		}

		[STAThread]
		[LoaderOptimization(LoaderOptimization.MultiDomainHost)]
		static void Main() 
		{
			try
			{
				m_form = new MainForm();
				Application.Run(m_form);
			}
			catch (Exception e)
			{
				Console.WriteLine("{0} {1} \n {2}", e.Message, e.StackTrace, e.ToString());
			}
		}

		static	MainForm		m_form = null;
		string	m_appsFile =	"apps.txt";
		string	m_appsPath;
	}
}
