// CosurferForm.cs
namespace Cosurfer
{    
	using System;
	using System.Drawing;
	using System.ComponentModel;
	using System.Windows.Forms;

	/// <summary>
	///    Summary description for CosurferForm.
	/// </summary>
	public class MainForm : 
		System.Windows.Forms.Form,
		ICosurfEngineEvents
	{
		//private System.ComponentModel.Container components;
		private System.Windows.Forms.TextBox textBox_url;
		private System.Windows.Forms.TextBox textBox_chatContents;
		private System.Windows.Forms.Button  buttonConnect;
		private System.Windows.Forms.TextBox textBox_chatLine;
		private Cosurfer.CosurfEngine   m_engine; 

		public MainForm(CosurfEngine engine) 
		{
			m_engine = engine;
			InitializeComponent();

			Application.ApplicationExit += new EventHandler(this.OnApplicationExit);
		}

		public void OnApplicationExit(Object sender, EventArgs ea)
		{
			Console.WriteLine("CosurferForm::OnApplicationExit()");
		}

		/// <summary>
		///    Required method for Designer support - do not modify
		///    the contents of this method with an editor
		/// </summary>
		private void InitializeComponent()
		{
			this.textBox_url = new System.Windows.Forms.TextBox();
			this.buttonConnect = new System.Windows.Forms.Button();
			this.textBox_chatLine = new System.Windows.Forms.TextBox();
			this.textBox_chatContents = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// textBox_chatContents
			// 
			this.textBox_chatContents.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.textBox_chatContents.Location = new System.Drawing.Point(8, 8);
			this.textBox_chatContents.Multiline = true;
			this.textBox_chatContents.Name = "textBox_chatContents";
			this.textBox_chatContents.ReadOnly = true;
			this.textBox_chatContents.Size = new System.Drawing.Size(316, 240);
			this.textBox_chatContents.TabIndex = 1;
			this.textBox_chatContents.Text = "";

			// 
			// textBox_url
			// 
			this.textBox_url.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.textBox_url.Location = new System.Drawing.Point(8, 288);
			this.textBox_url.Name = "textBox_url";
			this.textBox_url.Size = new System.Drawing.Size(232, 20);
			this.textBox_url.TabIndex = 3;
			this.textBox_url.Text = "127.0.0.1";
			// 
			// buttonConnect
			// 
			this.buttonConnect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonConnect.Location = new System.Drawing.Point(248, 288);
			this.buttonConnect.Name = "buttonConnect";
			this.buttonConnect.TabIndex = 0;
			this.buttonConnect.Text = "Connect";
			this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
			// 
			// textBox_chatLine
			// 
			this.textBox_chatLine.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.textBox_chatLine.Location = new System.Drawing.Point(8, 256);
			this.textBox_chatLine.Name = "textBox_chatLine";
			this.textBox_chatLine.Size = new System.Drawing.Size(316, 20);
			this.textBox_chatLine.TabIndex = 2;
			this.textBox_chatLine.Text = "Enter Your Text Here...";
			this.textBox_chatLine.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox_chatLine_KeyPress);
			// 
			// MainForm
			// 
			this.AutoScale = false;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(330, 317);
			this.Controls.Add(this.textBox_url);
			this.Controls.Add(this.textBox_chatContents);
			this.Controls.Add(this.textBox_chatLine);
			this.Controls.Add(this.buttonConnect);
			this.Name = "MainForm";
			this.Opacity = 0.7;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.Text = "Cosurfer";
			this.TopMost = true;
			this.ResumeLayout(false);

		}

		protected override void OnClosed(System.EventArgs e)
		{
			if (m_engine != null)
				m_engine.Dispose();
			
			base.OnClosed(e);
		}   

		protected void buttonConnect_Click(object sender, System.EventArgs e)
		{
			((Button)sender).Enabled = false;
			string url = textBox_url.Text;
			m_engine.Connect(url);
			this.Text = "Cosurfer - Connected";
			m_engine.LaunchBrowser();
		}

		public void OnStartListening(CosurfEngine engine, int port)
		{
			Console.WriteLine("CosurferForm::OnStartListening()");		
			// set the caption dynamically
			this.Text = "Cosurfer - Listening on port :" + port;
		}
		
		public void OnConnectionAccepted(CosurfEngine engine)
		{
			Console.WriteLine("CosurferForm::OnConnectionAccepted()");
			buttonConnect.Enabled = false;
			m_engine.LaunchBrowser();
			this.Text = "Cosurfer - Connected";
		}

		public void OnDisconnect(CosurfEngine engine)
		{
			Console.WriteLine("CosurferForm::OnDisconnect()");
			this.Text = "Cosurfer - Disonnected";
			buttonConnect.Enabled = true;
		}

		public void OnIncomingChatLine(CosurfEngine engine, String chatLine)
		{
			textBox_chatContents.AppendText(chatLine);
			textBox_chatContents.AppendText("\r\n");
		}

		private void textBox_chatLine_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			String chatLine = textBox_chatLine.Text;
			if ( e.KeyChar != 13 || chatLine.Length == 0)
				return;

			m_engine.SendChatLine(chatLine);
			OnIncomingChatLine(m_engine, chatLine);		
		}
	}
}
