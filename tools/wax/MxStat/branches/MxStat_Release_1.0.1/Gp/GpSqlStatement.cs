using System;
using System.Collections.Generic;
using System.Text;

namespace MxStat.Gp
{
	public class GpSqlStatement : GpText
	{
		string idWaxDateVon = "*WaxDateVon*",
			   idWaxDateBis = "*WaxDateBis*",
			   idWanr = "*Wanr*",
			   idWaid = "*waid*",
			   idSdAuftrag = "*SdAuftrag*",
			   idSdPosVon = "*SdPosVon*",
			   idSdPosBis = "*SdPosBis*";


		DateTime datumVon,
			   datumBis;


		public GpSqlStatement()
			: base(null)
		{
		}


		public GpSqlStatement(string txt) : base(txt)
		{
		}

		public void setSqlText(string txt)
		{
			this.Text = txt;
		}
		
		public DateTime DatumVon
		{
			set
			{
				datumVon = value;
			}
		}

		public DateTime DatumBis
		{
			set
			{
				datumBis = value;
			}
		}

		public bool hasWaxDateVon()
		{
			if (this.Text.IndexOf(idWaxDateVon) >= 0)
				return true;

			return false;
		}

		public bool hasWaxDateBis()
		{
			if (this.Text.IndexOf(idWaxDateBis) >= 0)
				return true;

			return false;
		}

		public bool hasWanr()
		{
			if (this.Text.IndexOf(idWanr) >= 0)
				return true;

			return false;
		}

		public bool hasWaid()
		{
			if (this.Text.IndexOf(idWaid) >= 0)
				return true;

			return false;
		}

		public bool hasSdAuftrag()
		{
			if (this.Text.IndexOf(idSdAuftrag) >= 0)
				return true;

			return false;
		}

		public bool hasSdPosVon()
		{
			if (this.Text.IndexOf(idSdPosVon) >= 0)
				return true;

			return false;
		}

		public bool hasSdPosBis()
		{
			if (this.Text.IndexOf(idSdPosBis) >= 0)
				return true;

			return false;
		}

		public bool insertWaxDateVon(DateTime date)
		{
			return this.insertWaxDateAbrunden(idWaxDateVon, date);
		}

		public bool insertWaxDateBis(DateTime date)
		{
			return this.insertWaxDateAufrunden(idWaxDateBis, date);
		}

		public bool insertWanr(string wanr)
		{
			if (String.IsNullOrEmpty(wanr))
				return false;

			return this.insertWert(idWanr, wanr);
		}

		public bool insertWaid(string wanr)
		{
			if (String.IsNullOrEmpty(wanr))
				return false;

			return this.insertWert(idWaid, wanr);
		}

		public bool insertSdAuftrag(string nr)
		{
			if (String.IsNullOrEmpty(nr))
				return false;

			return this.insertWert(idSdAuftrag, nr);
		}

		public bool insertSdPosVon(string nr)
		{
			if (String.IsNullOrEmpty(nr))
				return false;

			return this.insertWert(idSdPosVon, nr);
		}

		public bool insertSdPosBis(string nr)
		{
			if (String.IsNullOrEmpty(nr))
				return false;

			return this.insertWert(idSdPosBis, nr);
		}
	}
}
