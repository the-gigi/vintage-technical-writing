using System;
using System.Xml;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;

namespace XmlSpike
{
	class App
	{
		[DllImport("msvcrt.dll")]
		static extern int _getch();

		[STAThread]
		static void Main(string[] args)
		{
			string xml = @"<?xml version=""1.0""?>
<Document>
	<Frame Url=""http://www.w3schools.com/tags/planets.htm"">
		<Frame Url=""http://www.w3schools.com/tags/venus.htm"" />
		<Frame Url=""http://www.w3schools.com/tags/sun.htm"" />
		<Frame Url=""http://www.w3schools.com/tags/mercur.htm"" />
	</Frame>
</Document>
";


			xml = CreateXml();
			ParseXml(xml);

			_getch();
		}

		static string CreateXml()
		{
			StringWriter sw = new StringWriter();
			XmlTextWriter w = new XmlTextWriter(sw);
			w.IndentChar = '\t';
			w.Indentation = 1;
			w.Formatting = Formatting.Indented;

			w.WriteProcessingInstruction("xml", @"version=""1.0""");
			w.WriteStartElement("Document");
				w.WriteStartElement("Frame");
				w.WriteAttributeString("Url", "http://www.w3schools.com/tags/planets.htm");
					w.WriteStartElement("Frame");
					w.WriteAttributeString("Url", "http://www.w3schools.com/tags/venus.htm");
					w.WriteEndElement();
			
					w.WriteStartElement("Frame");
					w.WriteAttributeString("Url", "http://www.w3schools.com/tags/sun.htm");
					w.WriteEndElement();

					w.WriteStartElement("Frame");
					w.WriteAttributeString("Url", "http://www.w3schools.com/tags/mercur.htm");
					w.WriteEndElement();

				w.WriteEndElement();
			w.WriteEndElement();
			
			return sw.ToString();
		}

		static void ParseXml(string xml)
		{
				XmlDocument doc = new XmlDocument();
				doc.LoadXml(xml);
				XmlElement root = doc.DocumentElement;
				DumpNode(root.FirstChild, "");				
		}

		static void DumpNode(XmlNode node, string indent)
		{
			XmlAttribute url = node.Attributes.GetNamedItem("Url") as XmlAttribute;
			Console.WriteLine("{0}{1}", indent, url.Value.ToString());
			foreach (XmlNode child in node.ChildNodes)
				DumpNode(child, indent + "\t");
		}
	}
}
