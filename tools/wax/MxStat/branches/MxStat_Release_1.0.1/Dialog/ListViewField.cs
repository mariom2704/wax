using System;
using System.Collections.Generic;
using System.Text;

namespace MxStat.Dialog
{
    public class ListViewField
    {
        Object mTag = null;

        FieldStyle mFieldStyle = FieldStyle.ReadOnly;

        int mFarbe = 0;

        //Feldwerte
        string mString = "";
        double mDouble = 0;
        int mInt = 0;


        //Combobox Listen
        List<string>  mStringliste = null;
        List<int>  mIntliste = null;
        List<double>  mDoubleliste = null;

        string mComboKeineauswahl = "";
        string mComboVorgabe = "";


        int mNachkomma = 2;

        FieldVarTyp mVarTyp = FieldVarTyp.STRING;


        enum FieldVarTyp
        {
            STRING,
            INT,
            DOUBLE
        };

        public enum FieldStyle {
            ReadOnly,
            Edit,
            DropDown,
            DropDownList
        };

        //Attribute -----------------------------

        public ListViewField()
        {
        }

        public Object Tag
        {
            get
            {
                return mTag;
            }
            set
            {
                mTag = value;
            }
        }

        public FieldStyle ListFieldStyle
        {
            get
            {
                return mFieldStyle;
            }
            set
            {
                mFieldStyle = value;
            }
        }


        public int Farbe
        {
            get
            {
                return mFarbe;
            }
            set
            {
                mFarbe = value;
            }
        }

  
	    public string ComboBoxKeineAuswahl
        {
            get{
                return mComboKeineauswahl;
            }
            set{
                mComboKeineauswahl = value;
            }
        }


        public string ComboBoxVorgabe
        {
            get{
                return mComboVorgabe;
            }
            set{
                mComboVorgabe = value;
            }
        }

        //Methoden

        public void setWert(string wert,bool readOnly)
        {
            mString = wert;
            mVarTyp = FieldVarTyp.STRING;

            if (readOnly)
                mFieldStyle = FieldStyle.ReadOnly;
            else
                mFieldStyle = FieldStyle.Edit;

        }

        public void setWert(int wert,bool readOnly)
        {
            mInt = wert;
            mVarTyp = FieldVarTyp.INT;

            if (readOnly)
                mFieldStyle = FieldStyle.ReadOnly;
            else
                mFieldStyle = FieldStyle.Edit;
        }

        public void setWert(double wert, int nachkomma,bool readOnly)
        {
            mDouble = wert;
            mVarTyp = FieldVarTyp.DOUBLE;
            mNachkomma = nachkomma;

            if (readOnly)
                mFieldStyle = FieldStyle.ReadOnly;
            else
                mFieldStyle = FieldStyle.Edit;
        }

        


	    public void setComboBoxWerte(List<string> werte,bool readOnly)
        {
            if (readOnly)
                mFieldStyle = FieldStyle.DropDown;
            else
                mFieldStyle = FieldStyle.DropDownList;

            //Liste kopieren
            mStringliste.Clear();
            mStringliste.AddRange(werte);
        }

	    public void setComboBoxWerte(List<int> werte,bool readOnly)
        {
            
            if (readOnly)
                mFieldStyle = FieldStyle.DropDown;
            else
                mFieldStyle = FieldStyle.DropDownList;

            //Liste kopieren
            mIntliste.Clear();
            mIntliste.AddRange(werte);
        }

	    public void setComboBoxWerte(List<double> werte,int anzNachkomma,bool readOnly)
        {
            if (readOnly)
                mFieldStyle = FieldStyle.DropDown;
            else
                mFieldStyle = FieldStyle.DropDownList;

            //Liste kopieren
            mDoubleliste.Clear();
            mDoubleliste.AddRange(werte);

            mNachkomma = anzNachkomma;
        }


        public override string ToString()
        {
            string str = "";

            if (mFieldStyle == FieldStyle.Edit || mFieldStyle == FieldStyle.ReadOnly)
            {
                switch (mVarTyp)
                {
                    case FieldVarTyp.INT:
                        str = mInt.ToString();
                        break;
                    case FieldVarTyp.DOUBLE:
                        str = mDouble.ToString();   //ToDo
                        break;
                    default:
                        str = mString;
                        break;
                }
            }
            else
            {
                //ToDo
            }

            return str;
        }
    }
}
