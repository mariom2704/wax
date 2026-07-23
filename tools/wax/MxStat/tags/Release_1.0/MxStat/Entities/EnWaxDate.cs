using System;
using System.Collections.Generic;
using System.Text;

namespace MxStat.Entities
{
    public class EnWaxDate
    {
        long mWaxDate;

        public EnWaxDate(DateTime date)
        {
            setDate(date);
        }

        public EnWaxDate()
        {
        }

        public long Value
        {
            get
            {
                return mWaxDate;
            }
            set
            {
                mWaxDate = value;
            }

        }

        public void setDate(DateTime date)
        {
            long diffDays;


            //BasisDatum COleDateTime
            DateTime oleDate = new DateTime(1899,12,30);

            //Tage zum Basisdatum 
            TimeSpan span = date - oleDate;
            diffDays = span.Days;

            //umrechnen in Wax-Datum
            mWaxDate = diffDays * 10000;
        }


        public DateTime getDate()
        {

            //auf diesem Datum basiert das COleDateTime Objekt. 
            DateTime date = new DateTime(1899,12,30);

            //Wax Datum ins COleDateTime Basisdatum umrechnen
            long waxDate = mWaxDate / 10000;

            TimeSpan span = new TimeSpan((int)waxDate, 0, 0, 0);

            //Tage anhängen
            date += span;

            return date;
        }

        /// <summary>
        /// Auf Tagesanfang abrunden
        /// </summary>
        public void abrunden()
        {
            mWaxDate = (mWaxDate / 10000) * 10000;
        }

        /// <summary>
        /// auf Tagesende aufrunden
        /// </summary>
        public void aufrunden()
        {
            mWaxDate = (mWaxDate / 10000) * 10000 + 9999;
        }

        public override string ToString()
        {
            return mWaxDate.ToString();
        }
    }
}
