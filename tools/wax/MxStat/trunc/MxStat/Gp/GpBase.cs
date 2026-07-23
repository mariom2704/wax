using System;
using System.Collections.Generic;
using System.Text;
using MxLib.Basis;

namespace MxStat.Gp
{
    public class GpBase : IMxError
    {
        MxErrorListe mErrorliste = new MxErrorListe();

        public MxErrorListe Errorliste
        {
            get
            {
                return mErrorliste;
            }
        }

        public void appendErrorListe(IMxError errorliste)
        {
            mErrorliste.AddRange(errorliste.Errorliste);
        }
    }
}
