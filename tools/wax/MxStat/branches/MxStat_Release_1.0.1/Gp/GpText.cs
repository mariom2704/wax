using System;
using System.Collections.Generic;
using System.Text;
using MxStat.Entities;

namespace MxStat.Gp
{
	public class GpText
	{
		string text;


		public GpText(string txt)
		{
			text = txt;
		}

		public string Text
		{
			get { return text; }
			set { text = value; }
		}


		public bool hasTag(string tag)
		{
			return text.StartsWith(tag);
		}

		public bool insertWert(string tag, string wert)
		{
			text = text.Replace(tag, wert);

			return true;
		}


		public bool insertWaxDateAbrunden(string tag, DateTime date)
		{
			EnWaxDate waxDate = new EnWaxDate(date);

			//Abrunden
			waxDate.abrunden();
			
			insertWert(tag, waxDate.ToString());

			return true;
		}

		public bool insertWaxDateAufrunden(string tag, DateTime date)
		{
			EnWaxDate waxDate = new EnWaxDate(date);

			//Aufrunden
			waxDate.aufrunden();
			
			insertWert(tag, waxDate.ToString());

			return true;
		}

		public bool insertWaxDate(string tag, DateTime date)
		{
			EnWaxDate waxDate = new EnWaxDate(date);

			insertWert(tag, waxDate.ToString());

			return true;
		}
	}
}
