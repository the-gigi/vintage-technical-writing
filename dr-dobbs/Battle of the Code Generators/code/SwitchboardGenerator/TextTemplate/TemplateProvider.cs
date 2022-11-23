using System;
using System.Text;
using System.IO;

namespace TextTemplate
{
	/// <summary>
	/// Stores all the templates in memory for quick access
	/// </summary>
	public class TemplateProvider
	{
		static TemplateProvider()
		{
			const string templateDir = @"..\..\EventDemo\SwitchboardGenerator\TextTemplate";
			StreamReader tr = new StreamReader(Path.Combine(templateDir, "SwitchboardTemplate.txt"));
			m_switchboardTemplate = tr.ReadToEnd();

			tr = new StreamReader(Path.Combine(templateDir, "EventTemplate.txt"));
			m_eventTemplate = tr.ReadToEnd();

			tr = new StreamReader(Path.Combine(templateDir, "SubscribeUnsubscribeTemplate.txt"));
			m_subscribeUnsubscribeTemplate = tr.ReadToEnd();
		}
	
		static public string GetEventTemplate()
		{
			return m_eventTemplate;
		}

		static public string GetSwitchboardTemplate()
		{
			return m_switchboardTemplate;
		}

		static public string GetSubscribeUnsubscribeTemplate()
		{
			return m_subscribeUnsubscribeTemplate;
		}

		private static string m_eventTemplate;
		private static string m_switchboardTemplate;
		private static string m_subscribeUnsubscribeTemplate;
	}  	
}
