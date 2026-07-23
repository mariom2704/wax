using System;
using System.Collections.Generic;
using System.Text;
using System.Data.Common;
using System.Windows.Forms;


using MxStat.Entities;

namespace MxStat.Db
{
    public class DbSqlBase
    {

        DbConnection mDbCon = new DbConnection();

        public DbSqlBase()
        {
        }

        public DbConnection Connection
        {
            get
            {
                return mDbCon;
            }
        }


        public bool executeSql(string sqlStatement, EnMatrix matrix)
        {
            if (matrix == null)
                return false;

            matrix.clearField();

            bool res = true;

            DbDataReader reader = openDbAndGetReader(sqlStatement);
            if (reader == null)
                return false;


            try
            {
                //Abfragewerte in Liste speichern
                int spalte = 0,
                    zeile = 0;
                while (reader.Read())
                {
                    spalte = 0;
                    for (int i = 0; i < reader.FieldCount; i++)
                    {
                        //Spaltenname setzen
                        matrix.setColumnName(i, reader.GetName(i));
                        
                        //Feld setzen
                        matrix.setAt(zeile, spalte,reader.GetValue(i));
                        spalte++;
                    }

                    zeile++;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Fehler bei Db Zugriff:" + ex.Message);
                res = false;
            }
            finally
            {
                if (reader != null) reader.Close();
            }

            //Db schließen
            this.Connection.closeDb();


            return res;
        }


        /// <summary>
        /// Für Sql Abfragen mit einem Satz als Antwort
        /// </summary>
        /// <param name="sqlStatement"></param>
        /// <param name="result"></param>
        /// <returns></returns>
        public bool executeSqlFirstRow(string sqlStatement, List<string> result)
        {
            result.Clear();

            bool res = true;

            DbDataReader reader = openDbAndGetReader(sqlStatement);
            if (reader == null)
                return false;


            try
            {
                //Abfragewerte in Liste speichern
                if (reader.Read())
                {
                    for (int i = 0; i < reader.FieldCount; i++)
                    {
                        result.Add(reader.GetValue(i).ToString());
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Fehler bei Db Zugriff: " + ex.Message);
                res = false;
            }
            finally
            {
                if (reader != null) reader.Close();
            }

            //Db schließen
            this.Connection.closeDb();


            return res;
        }


        /// <summary>
        /// Datenbank öffnen, Sql Statement ausführen und Reader auf Abfrage zurückgeben
        /// </summary>
        /// <param name="sqlStatement"></param>
        /// <returns></returns>
        protected DbDataReader openDbAndGetReader(string sqlStatement)
        {
            //Datenbank öffnen
            if (!this.Connection.openDb())
                return null;

            DbCommand cmd = this.Connection.getCommand(sqlStatement);

            DbDataReader reader = null;

            try
            {
                //Abfrage starten
                reader = cmd.ExecuteReader();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Fehler bei Db Zugriff: " + ex.Message);
                
                if (reader != null) reader.Close();
                reader = null;
            }

            //wenn Fehler, dann Db wieder schließen
            if (reader == null)
                this.Connection.closeDb();

            return reader;
        }


        public bool createTableAllColumnsAsString(string tabName, List<string> spalten)
        {
            if (spalten.Count == 0)
                return false;

            string typeName = "TEXT";

            //Sql Statement erstellen
            string stSql = String.Format("CREATE TABLE `{0}` (", tabName);

            //Tabellenfelder an Statement anhängen
            int i = 0;
            foreach (string spalte in spalten)
            {
                if (i > 0)
                    stSql += ",";

                stSql += String.Format("`{0}` {1}",spalte,typeName);
                i++;
            }

            stSql += ")";


            //Statement ausführen
            DbDataReader reader = openDbAndGetReader(stSql);
            if (reader == null)
                return false;

            return true;
        }

        public bool insertInto(string tabName, List<string> spalten, List<object> values)
        {
            if (spalten.Count != values.Count)
                return false;

            //Sql Statement erstellen
            string stSql = String.Format("INSERT INTO `{0}` (", tabName);

            //Tabellenfelder an Statement anhängen
            for (int i = 0; i < spalten.Count; ++i)
            {
                if (i > 0)
                    stSql += ",";

                stSql += String.Format("`{0}`",spalten[i]);
            }

            stSql += ") VALUES (";

            //Values
            for (int i = 0; i < values.Count; ++i)
            {
                if (i > 0)
                    stSql += ",";


				if (values[i] != null)
					stSql += String.Format("'{0}'", values[i].ToString());
				else
					stSql += "''";
            }

            stSql += ")";


            //Statement ausführen
            DbDataReader reader = openDbAndGetReader(stSql);
            if (reader == null)
                return false;

            return true;
        }

        public bool insertInto(string tabName, EnMatrix matrix)
        {
            List<object> liste = null;

            for (int row = 0; row < matrix.MaxRow; row++)
            {
                liste = new List<object>();
                for (int col = 0; col < matrix.MaxColumn; col++)
                {
                    
                    liste.Add(matrix.getAt(row, col));
                }

                if (!insertInto(tabName, matrix.ColumnNames, liste))
                    return false;
            }

            return true;
        }
    }
}
