using System;
using System.Collections.Generic;
using System.Text;
using Autodesk.AutoCAD.Runtime;
using MxLib.Basis;


namespace Utils.Befehle
{
    public class BefehlBase : IMxError
    {
        public bool start()
        {
            return startCommand();
        }

        protected virtual bool startCommand()
        {
            return false;
        }


        MxErrorListe mErrorliste = new MxErrorListe();

        #region IMxError Member

        public MxErrorListe Errorliste
        {
            get { return mErrorliste; }
        }

        public void appendErrorListe(IMxError errorliste)
        {
            mErrorliste.AddRange(errorliste.Errorliste);
        }

        #endregion
    }
}
