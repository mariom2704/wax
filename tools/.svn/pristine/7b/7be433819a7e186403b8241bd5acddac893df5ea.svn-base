using System;
using System.Collections.Generic;
using System.Text;
using RegChange.En;
using Microsoft.Win32;

namespace RegChange.Gp
{
    public class GpRegistry
    {
        string message = "";

        public string Message
        {
            get { return message; }
        }

        public bool appendValue(string key, string var, string separator, List<string>deleteVal, List<EnRegVal>newVal)
        {
            object res = Registry.GetValue(key, var, null);
            if (res == null)
                throw new Exception(String.Format("Der Eintrag {0} oder der Schlüssel {1} wurden nicht gefunden.", var, key));

            string value = res as String;
            if (value == null)
                throw new Exception(String.Format("Ungültiger Wert {2} bei dem Eintrag {0} unter dem Schlüssel {1}.", var, key, res));

            List<EnRegVal>vals = new List<EnRegVal>(newVal);

            //String aufsplitten
            string[] sep = { separator };
            string[] split = value.Split(sep, StringSplitOptions.RemoveEmptyEntries);
            
            //Liste um delete Einträge bereinigen
            List<string> newSplit = new List<string>();
            foreach (string st in split)
            {
                string stLow = st.ToLower();                  
                bool found = false;
                foreach (string stDel in deleteVal)
                {
                    if (stDel == "*")
                    {
                        found = true;
                        break;
                    }

                    if (stLow.LastIndexOf(stDel.ToLower()) >= 0)
                    {
                        found = true;
                        break;
                    }
                }

                //Wenn delete Eintrag gefunden wurde, diesen überspringen
                if (found)
                    continue;
               
                newSplit.Add(st);
            }

                      
            //neue Werte einfügen           
            foreach (EnRegVal regVal in vals)
            {
                int pos = regVal.Position;
                if (regVal.Position < 0)
                    pos = 0;
                else if (newSplit.Count < regVal.Position)
                    pos = newSplit.Count;

                newSplit.Insert(pos, regVal.Val);
            }

            //als String
            string newValue = "";
            foreach (string st in newSplit)
            {
                append(ref newValue, st, separator);
            }

            //Key neu schreiben
            Registry.SetValue(key, var, newValue);

            message = "\nNeuer Eintrag:\n" + newValue;

            return true;
        }


        void append(ref string val, string ap, string sep)
        {
            if (String.IsNullOrEmpty(val))
                val = ap;
            else
                val += sep + ap;
        }

        
    }
}
