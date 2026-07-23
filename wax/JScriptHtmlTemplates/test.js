function tdr(){

    var cell = document.createElement("p");
    var strImagePath = 'C:\\TEMP\\wax\\5e8b.dwf';
    cell.innerHTML = "<object id='ADViewer'  classid='clsid:A662DA7E-CCB7-4743-B71A-D817F6D575DF'>" +
                "<param name='Src' value='" + strImagePath + "'/></object>"; 
   document.body.appendChild(cell);
  
  /*  
    var m_DWFImage = document.createElement('object');
    m_DWFImage.width = "100%";
    m_DWFImage.height = "400";
    embedTagSVG.innerHTML("param name='src'", "http://download.autodesk.com/global/dwf/gallery/files/085825-1-zobell_residence.dwf");
    m_DWFImage.setAttribute('classid', 'clsid:A662DA7E-CCB7-4743-B71A-D817F6D575DF');
    var cell = document.body.appendChild(embedTagSVG);
    */
  //  m_DWFImage.setAttribute("id", "ADViewer");
   
}

  
