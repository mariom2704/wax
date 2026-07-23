using System;
using System.Collections.Generic;
using System.Text;

namespace MxStat.Entities
{
    public class EnMatrix
    {
        Dictionary<string, Object> mField;
        List<string> mSpaltennames;

        int maxSpalten = -1,
		    maxZeilen  = -1;

        int keySpalte = -1;  //Spaltennummer der Spalte die den Key beinhaltet



        public EnMatrix()
        {
            mField = new Dictionary<string,object>();
            mSpaltennames = new List<string>();
        }

        //Attribute-------------------------------------------------------------------

        public int MaxColumn
        {
            get
            {
                return maxSpalten + 1;
            }

        }


        public int MaxRow									//Größte Zeilennummer
        {
            get
            {
                return maxZeilen + 1;
            }
        }


        public int KeySpalte
        {
            get
            {
                return keySpalte;
            }
            set
            {
                keySpalte = value;
            }
        }


       
        /// <summary>
        /// Liste mit Columnnames zurückgeben, wenn Liste kleiner MaxColumn ist, wird diese Aufgefüllt
        /// </summary>
        public List<string> ColumnNames
        {
            get
            {
                if (mSpaltennames.Count < MaxColumn)
                {
                    //Spalten mit Dummys auffüllen
                    for (int i = mSpaltennames.Count+1; i <= MaxColumn; i++)
                        mSpaltennames.Add("");
                }

                return mSpaltennames;
            }
        }


        //Methoden--------------------------------------------------------------------

        public void setAt(int zeile,int spalte,Object value)	//Eintrag hinzufügen
        {
            if (zeile < 0 || spalte < 0)
		        return;

	        //Max Werte aktualisieren
	        if (zeile > maxZeilen)
		        maxZeilen = zeile;

	        if (spalte > maxSpalten)
		        maxSpalten = spalte;

            //Wert Evt. vorher löschen
            mField.Remove(getKey(zeile, spalte));

	        //Wert speichern
	        mField.Add(getKey(zeile,spalte),value);

        }

        public Object getAt(int zeile, int spalte)	//Eintrag holen
        {
            if (zeile < 0 || spalte < 0)
                return null;
           
            Object result = null;

            try
            {
              result = mField[getKey(zeile, spalte)];
            }
            catch
            {
                //Key wurde nicht gefunden
                return null;
            }

            //DbNull Werte abfangen
            if (result == System.DBNull.Value)
                return null;
                        
            return result;
        }


        public void setColumnName(int spalte, string name)
        {
            if (spalte < 0 /*|| spalte > maxSpalten*/)
                return;

            if (spalte >= mSpaltennames.Count)
            {
                //Spalten mit Dummywerten auffüllen
                for (int i = mSpaltennames.Count; i <= spalte; i++)
                    mSpaltennames.Add("");
            }

            mSpaltennames[spalte] = name;
        }


        /// <summary>
        /// Matrix zurücksetzen
        /// </summary>
        public void clearField()
        {
            mField.Clear();
            maxZeilen = -1;
        }

        /// <summary>
        /// Matrix und Spalten zurücksetzen
        /// </summary>
        public void clearAll()
        {
            clearField();
            mSpaltennames.Clear();
            maxSpalten = -1;
            keySpalte = -1;
        }



        /// <summary>
        /// in Zeile "zeile" neuen Eintrag setzen. Wenn die Spalte "spaltenname" noch nicht existiert, wird sie 
        /// in mSpaltennames angelegt
        /// </summary>
        /// <param name="spaltenname"></param>
        /// <param name="value"></param>
        public void set(int zeile,string spaltenname, Object value)
        {
            int spalte = 0;

            //Prüfen ob Spalte schon vorhanden ist;
            bool found = false;
            for (spalte = 0; spalte < mSpaltennames.Count; spalte++)
            {
                if (spaltenname == mSpaltennames[spalte])
                {
                    found = true;
                    break;
                }
            }

            //Wenn Spalkte noch nicht vorhanden ist, diese definieren
            if (!found)
            {
                mSpaltennames.Add(spaltenname);
                spalte = mSpaltennames.Count - 1;
            }

            //Wert speichern
            setAt(zeile, spalte, value);
        }



        //protected Methoden---------------------------------------------------------------



        protected string getKey(int zeile, int spalte)
        {
            return spalte.ToString() + "_" + zeile.ToString();
        }
    }
}
