using System;
using System.Collections.Generic;
using System.Text;

namespace UpdateWerte.En
{
	public class StringPair
	{
		string id;
		string bezeichnung;

		public StringPair()
		{
		}

		public StringPair(string id, string bez)
		{
			this.id = id;
			this.bezeichnung = bez;
		}

		public string Id
		{
			get { return id; }
			set { id = value; }
		}
		public string Bezeichnung
		{
			get { return bezeichnung; }
			set { bezeichnung = value; }
		}

		public override string ToString()
		{
			return Bezeichnung;
		}
	}
}
