using System;
using System.Collections.Generic;
using System.Text;
using MxLib.Basis;
using SapMatToWax.En;

namespace SapMatToWax.Gp
{
    public class GpSapMaterialstamm
    {
        string sapdatei = null;

        const int anzTabArtikelnummer = 0;
        const int anzTabBezeichnungDeutsch = 1;
        const int anzTabPreis = 2;
        const int anzTabPreiseinheit = 3;
        const int anzTabEinheit = 4;
        //Bruttogewicht = 5
        const int anzTabGewicht = 6;
        //Gewichtseinheit = 7
        //letze Änderung Name = 8
        //letzte Änderung Datum = 9
        const int anzTabStatus = 10;
        //Status gesetzt am (Datum) = 11
        //Löschvormerkung = 12
        const int anzTabBezeichnungEnglisch = 13;


       
        MxErrorListe errorListe = new MxErrorListe();

        public GpSapMaterialstamm(string datei)
        {
            sapdatei = datei;  
        }

        public bool einlesen(List<EnArtikel> artikel)
        {
            TextDatei datei = new TextDatei(sapdatei);

            List<string>zeilen = new List<string>();
            if (!datei.read(zeilen, Encoding.Default))
            {
                ErrorListe.AddRange(datei.Errorliste);
                return false;
            }

            foreach(string str in zeilen)
            {
                EnArtikel art = new EnArtikel();

                string[] split = str.Split(new Char[] {'\t'});

                int count = 0;
                int ival = 0;
                double dval = 0;
                string tmp;
                string bez = " ";
                double preisEinheit = 1;
                foreach (string st in split)
                {
                    //Leerzeichnen löschen
                    tmp = st.Trim();

                    switch (count)
                    {
                        case anzTabArtikelnummer:
                            if (int.TryParse(tmp, out ival))
                                art.Artikelnummer = ival;
                            break;
                        case anzTabBezeichnungDeutsch:
                            if (String.IsNullOrEmpty(tmp))
                                tmp = bez;
                            art.Bezeichnung = tmp;
                            bez = tmp;
                            break;
                        case anzTabBezeichnungEnglisch:
                            if (String.IsNullOrEmpty(tmp))
                                tmp = bez;
                            art.BezeichnungEng = tmp;
                            break;
                        case anzTabStatus:
                            if (int.TryParse(tmp, out ival))
                                art.Status = ival;
                            break;
                        case anzTabEinheit:
                            art.BezeichnungEinheit = tmp;
                            break;
                        case anzTabPreis:
                            tmp = tmp.Replace('.', ',');
                            if (double.TryParse(tmp, out dval))
                                art.SapPreis = dval;
                            break;
                        case anzTabGewicht:
                            tmp = tmp.Replace('.', ',');
                            if (double.TryParse(tmp, out dval))
                                art.GewichtProEinheit = dval;
                            break;
                        case anzTabPreiseinheit:
                            tmp = tmp.Replace('.', ',');
                            if (double.TryParse(tmp, out dval))
                                preisEinheit = dval;
                            break;
                    }

                    count++;
                }

                //Preiseinheit berücksichtigen
                art.SapPreis = art.SapPreis / preisEinheit;

                if (art.Artikelnummer > 0)
                    artikel.Add(art);
            }

            return true;
        }


        public MxErrorListe ErrorListe
        {
            get { return errorListe; }
            set { errorListe = value; }
        }
    }
}
