using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Drawing;
using System.Collections;

using MxStat.Entities;

namespace MxStat.Dialog
{
    public class ListViewExtendend : ListView
    {
        Dictionary<string, ListViewField> mField;

        bool mRowStyleWhiteGrey = false;  //Darstellung der Zeilen 

        Color colorGrey = Color.FromArgb(235, 235, 235);
        Color colorWhite = Color.White;


        public ListViewExtendend()
        {
            mField = new Dictionary<string, ListViewField>();

            //Standard Styles für dieses Control
            this.GridLines = true;
            this.View = System.Windows.Forms.View.Details;
        }



        /// <summary>
        /// Kompklette Liste und Spaltennamen über die Matrix setzen
        /// </summary>
        /// <param name="matrix"></param>
        public void setMatrix(EnMatrix liste)
        {
            if (liste == null)
                return;

            Object obj = null;

            //Liste löschen
            this.Clear();

            if (liste.MaxRow == 0)
                return;

            this.addColumns(liste.ColumnNames);

            //Spaltennummer mit dem Key
            int spalteArtNr = liste.KeySpalte;


            ListViewField field;


           

            //Matrix in ViewList einlesen
            for (int zeile = 0; zeile < liste.MaxRow; zeile++)
            {
                for (int spalte = 0; spalte < liste.MaxColumn; spalte++)
                {
                    obj = liste.getAt(zeile, spalte);
                    if (obj == null)
                        continue;

                    field = new ListViewField();
                    field.setWert(obj.ToString(), true);

                    //In der erstenSpalte den Key zuweisen
                    if (spalte == 0 && spalteArtNr >= 0 && spalteArtNr < liste.MaxColumn)
                    {
                        field.Tag = liste.getAt(zeile, spalteArtNr);
                    }

                    this.addField(zeile, spalte, field);
                }
            }

            
        }



        /// <summary>
        /// Eine Stringliste als Spalten hinzufügen
        /// </summary>
        /// <param name="liste"></param>
        public void addColumns(List<string> liste)
        {  
            ColumnHeader column;
            foreach (string str in liste)
            {
                column = new ColumnHeader();
                column.Text = str;
                this.Columns.Add(column);
            }
        }


        /// <summary>
        /// In Listcontrol ein Feld setzen
        /// </summary>
        /// <param name="spalte"></param>
        /// <param name="zeile"></param>
        /// <param name="field"></param>
        public void addField(int zeile, int spalte, ListViewField field)
        {
            //Range prüfen
            if (zeile < 0)
                return;
           
            if (spalte < 0)
                return;


            //Wert in der Matrix speichern
            mField.Remove(getKey(zeile, spalte));
            mField.Add(getKey(zeile, spalte), field);


            //Werte für Control setzen

            ListViewItem item;

            //evt. neue Items einfügen
            if (zeile >= this.Items.Count)
            {
                //mit Leerzeilen auffüllen
                for (int i=this.Items.Count; i<=zeile; i++) 
                {
                     item = new ListViewItem();
                     this.Items.Add(item);         
                }
            }

            //Item der Zeile holen
            item = this.Items[zeile];

            //Wenn grau / Weiß gesetzt ist, Frabe festlegen
            if (mRowStyleWhiteGrey)
            {
                if (zeile % 2 == 0)
                    item.BackColor = colorWhite;
                else
                    item.BackColor = colorGrey;
            }

            
            //subitems setzen
            int tmpSpalte = spalte;
            ListViewItem.ListViewSubItem subItem;

            if (tmpSpalte >= item.SubItems.Count)
            {
                //mit LeerSpalten auffüllen
                for (int i = item.SubItems.Count; i < tmpSpalte; i++)
                {
                    subItem = new ListViewItem.ListViewSubItem();
                    item.SubItems.Add(subItem);
                }

                //neues subitem anhängen
                subItem = new ListViewItem.ListViewSubItem();
                subItem.Text = field.ToString();
                subItem.Tag = field.Tag;
                item.SubItems.Add(subItem);

            }
            else
            {
                //vorhandenen Wert überschreiben
                item.SubItems[tmpSpalte].Text = field.ToString();
                item.SubItems[tmpSpalte].Tag = field.Tag;
            }

            //Wenn Spalte 0 gestezt wurde, dem Item einen evt. gesetzten Tag zuweisen
            if (spalte == 0)
                item.Tag = field.Tag;

        }


        protected string getKey(int zeile, int spalte)
        {
            return spalte.ToString() + "_" + zeile.ToString();
        }


        public bool RowStyleWhiteGrey
        {
            get
            {
                return mRowStyleWhiteGrey;
            }
            set
            {
                mRowStyleWhiteGrey = value;
            }
        }


        public void setColorWhiteGrey(Color white, Color grey)
        {
            colorWhite = white;
            colorGrey = grey;
        }



        /// <summary>
        /// Hintergrund abwechselnd auf weiß und grau setzen
        /// </summary>
        public void setBackColorWhiteGrey()
        {
            for (int i = 0; i < this.Items.Count; i++)
            {
                if (i % 2 == 0)
                    this.Items[i].BackColor = colorWhite;
                else
                    this.Items[i].BackColor = colorGrey;
            }
            
        }

    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Implements the manual sorting of items by columns.
    class ListViewItemComparer : IComparer
    {
        private int col;
        private SortOrder sortOrder = SortOrder.Descending;

        SortOrderMerker mSortOrderMerker = null;


        public ListViewItemComparer()
        {
            col = 0;
            sortOrder = SortOrder.Descending;
            mSortOrderMerker = null;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="column"></param>
        /// <param name="sortOrder"></param>
        /// <param name="compareTag">wenn der Tag des Items gesetzt ist, wird dieser anstatt des Itemtextes verglichen</param>
        public ListViewItemComparer(int column, SortOrder sortOrder)
        {
            col = column;
            this.sortOrder = sortOrder;
            mSortOrderMerker = null;
        }

        public ListViewItemComparer(int column, SortOrderMerker sortOrderMerker)
        {
            col = column;
            this.sortOrder = SortOrder.Descending;
            mSortOrderMerker = sortOrderMerker;
        }

        public int Compare(object x, object y)
        {

            if (mSortOrderMerker != null)
            {
                this.sortOrder = mSortOrderMerker.getSortOrder(col);
            }


            string xString = ((ListViewItem)x).SubItems[col].Text;
            string yString = ((ListViewItem)y).SubItems[col].Text;

            double xd, yd;
            int sort;

            //prüfen ob Wert evt. ein Double ist, da ein Stringvergleich auf double Werte einen falschen Vergleich liefert
            if (Double.TryParse(xString, out xd) == true &&
                Double.TryParse(yString, out yd) == true)
            {
                if (this.sortOrder == SortOrder.Descending)
                    sort = xd.CompareTo(yd);
                else
                    sort = yd.CompareTo(xd);
            }
            else
            {
                //Text vergleichen
                if (this.sortOrder == SortOrder.Descending)
                    sort = String.Compare(xString, yString);
                else
                    sort = String.Compare(yString, xString);
            }
            return sort;
        }
    }


    /////////////////////////////////////////////////////////////////////////////////////////////
    

    class SortOrderMerker
    {
        int mLastSortedColumn = -1,
            mCurrentLastSortedColumn = -1;

        SortOrder mLastSortOrder = SortOrder.Ascending,
                  mCurrentLastSortOrder = SortOrder.Ascending;

        public int LastSortedColumn
        {
            get
            {
                return mLastSortedColumn;
            }
            set
            {
                mLastSortedColumn = value;
            }
        }

        public SortOrder LastSortOrder
        {
            get
            {
                return mLastSortOrder;
            }
            set
            {
                mLastSortOrder = value;
            }
        }


        public SortOrder getSortOrder(int column)
        {
            SortOrder sortOrder = SortOrder.Descending;

            if (mLastSortedColumn == column)
            {
                if (mLastSortOrder == SortOrder.Descending)
                    sortOrder = SortOrder.Ascending;
                else
                    sortOrder = SortOrder.Descending;
            }

            //Werte merken
            mCurrentLastSortedColumn = column;
            mCurrentLastSortOrder = sortOrder;

            return sortOrder;
        }

        public void saveLastOrder()
        {
            mLastSortedColumn = mCurrentLastSortedColumn;
            mLastSortOrder = mCurrentLastSortOrder;
        }

    }
}
