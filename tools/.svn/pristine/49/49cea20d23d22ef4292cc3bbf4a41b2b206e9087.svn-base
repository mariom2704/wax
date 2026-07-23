using System;
using System.Collections.Generic;
using System.Text;
using MxLib.Db;
using MxLib.Entities;
using MxLib.Basis;

namespace UpdateWerte.Gp
{
	public class GpKonvertierung
	{
		string server1 = "",
			   server2 = "",
			   db1 = "",
			   db2 = "",
			   tabelle1 = "",
			   tabelle2 = "";

		List<string> spalten = null;
		List<string> keys = null;

		EnMatrix mat1 = new EnMatrix();

		MxErrorListe errorListe = new MxErrorListe();
	

		public GpKonvertierung(string server1, string datenbank1, string tab1,
							   string server2, string datenbank2, string tab2,
							   List<string> keys, List<string> spalten)
		{
			this.server1 = server1;
			this.server2 = server2;
			db1 = datenbank1;
			db2 = datenbank2;
			this.spalten = spalten;
			this.tabelle1 = tab1;
			this.tabelle2 = tab2;
			this.keys = keys;
		}

	
		public bool start(bool insertNewData, ref int changedRows, ref int insertRows)
		{
			if (!readDb1())
				return false;
			
			if (!writeDb2(insertNewData, ref changedRows, ref insertRows))
				return false;

			return true;
		}


		public MxErrorListe ErrorListe
		{
			get { return errorListe; }
			set { errorListe = value; }
		}

		bool readDb1()
		{
			EnConnectionData connData = new EnConnectionData();
			connData.Datenbank = db1;
            
			DbConnection conn1 = new DbConnection(connData);
            conn1.setDatabaseTyp();

			DbSqlBase abfrage = new DbSqlBase(conn1);
            abfrage.CloseDbByAccess = true;

			List<string> gesSpalten = new List<string>();
			gesSpalten.AddRange(keys);
			gesSpalten.AddRange(spalten);

			if (!abfrage.read(tabelle1, gesSpalten.ToArray(), null, mat1))
			{
				ErrorListe.AddRange(abfrage.Errorliste);
				return false;
			}

			return true;
		}


		bool writeDb2(bool insertNewData, ref int changedRows, ref int insertRows)
		{
			EnConnectionData connData = new EnConnectionData();
			connData.Datenbank = db2;
            
            DbConnection conn2 = new DbConnection(connData);
            conn2.setDatabaseTyp();

			DbSqlBase abfrage = new DbSqlBase(conn2);
            abfrage.CloseDbByAccess = true;

			if (!abfrage.updateTable(tabelle2, mat1, keys.ToArray(), insertNewData, ref changedRows, ref insertRows))
			{
				ErrorListe.AddRange(abfrage.Errorliste);
				return false;
			}


			return true;
		}

		
	}
}
