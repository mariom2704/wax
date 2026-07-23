using System;
using System.Collections.Generic;
using System.Text;
using System.Data.Odbc;
using System.Data.Common;
using System.Windows.Forms;

namespace MxStat.Db
{
    public class DbConnection
    {
        OdbcConnection mConnection = null;

        DbCommand mCommand = null;

        string mServer = "",
               mDatenbank = "",
               mUser = "",
               mPasswd = "";

        string mConnStr = "";

        DbTyp mDbTyp = DbTyp.nichtGesetzt;

        public enum DbTyp
        {
            nichtGesetzt = 0,
            Access = 1,
            MySql = 2,
            Excel = 3
        }

        public DbConnection()
        {
           
        }

        public DbTyp DatabaseTyp
        {
            set
            {
                mDbTyp = value;
            }
        }


        public OdbcConnection Connection
        {
            get
            {
                return mConnection;
            }
        }

        public string Datenbank
        {
            set
            {
                mDatenbank = value;
            }
        }


        public string Server
        {
            set
            {
                mServer = value;
            }
        }

        public string User
        {
            set
            {
                mUser = value;
            }
        }

        public string Passwort
        {
            set
            {
                mPasswd = value;
            }
        }



        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public bool openDb()
        {

            if (mConnStr.Length == 0)
            {
                if (string.IsNullOrEmpty(mDatenbank))
                    return false;

                switch (mDbTyp)
                {
                    case DbTyp.Access:
                        mConnStr = "Driver={Microsoft Access Driver (*.mdb)};DBQ=" + mDatenbank;
                        break;
                    case DbTyp.Excel:
                        mConnStr = "DRIVER={Microsoft Excel Driver (*.xls)};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=" + mDatenbank + ";DBQ=" + mDatenbank;
                        break;
                    case DbTyp.MySql:
                        mConnStr = String.Format("DRIVER={0};SERVER={1};DATABASE={2};USER={3};PASSWORD={4};OPTION=2072;STMT=;", 
                                                 "{MySQL ODBC 3.51 Driver}",mServer, mDatenbank, mUser, mPasswd);
                        break;
                    default:
                        return false;
                }

                
            }

            try
            {
                if (mConnection == null)
                    mConnection = new OdbcConnection(mConnStr);

                if (mConnection.State == System.Data.ConnectionState.Closed ||
                    mConnection.State == System.Data.ConnectionState.Broken)
                    mConnection.Open();
            }
            catch (OdbcException ex)
            {
                MessageBox.Show("Error connecting to the server: " + ex.Message);

                return false;
            }


            return true;
        }

        /// <summary>
        /// 
        /// </summary>
        public void closeDb()
        {
            if (mConnection != null)
                mConnection.Close();
        }


        public DbCommand getCommand(string sqlCommand)
        {
            mCommand = new OdbcCommand(sqlCommand, this.Connection);

            return mCommand;
        }


    }
}
