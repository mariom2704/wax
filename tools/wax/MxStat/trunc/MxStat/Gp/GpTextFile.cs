using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace MxStat.Gp
{
	class GpTextFile
	{
		string datei;

		string inhalt;

		public string getAsString()
		{
			return inhalt;
		}

		public string Datei
		{
			get { return datei; }
			set { datei = value; }
		}

		public bool readFile()
		{

			inhalt = "";

			try
			{
				// Create an instance of StreamReader to read from a file.
				// The using statement also closes the StreamReader.
				using (StreamReader sr = new StreamReader(datei))
				{
					String line;
					// Read and display lines from the file until the end of 
					// the file is reached.
					while ((line = sr.ReadLine()) != null)
					{
						if (String.IsNullOrEmpty(line))
							continue;

						line.Trim();

						inhalt += line + " ";
					}
				}
			}
			catch (Exception e)
			{
				// Let the user know what went wrong.
				MessageBox.Show(e.Message);
				return false;
			}

			return true;
		}
	}
}
