

 var m_ArrayOfAllImagePath;                // holds the path for DWF Images
 var m_ShowHeaders                  = 0;
 var m_CurrentHeader                = 'HeaderTable0';   
 var m_SplitterMarker               = '#####';
 var m_HochformatMarker             = '*****';
 var m_SplitterMarkerjpg            = '|||||';
 var m_SplitterCountPipes           = '+++++'; 
 var m_ChildBRLine                  = 1; 
 var m_ChildBarcode                 = 2; 
 var m_HeaderRows                   = 5;
 var m_bOnlineGraficInStaticLowerSplitter  = true;
 var m_ArrayOfSVGObjects = new Array();
 var m_ArrayOfDWFObjects = new Array();


 function scrollForPrint()
 {
    // For resizing the grafic, otherwise it stuck on the left size
     window.scrollBy(0, 10);
     window.scrollBy(0, -10);
 }

 

 function DrawingOnly(bOnlineGraficInStaticLowerSplitter)
 {
     m_bOnlineGraficInStaticLowerSplitter = bOnlineGraficInStaticLowerSplitter;
     window.scrollBy(0, -1000);
 }


 
 function ShowHeaderRows(strHeaderRows)
 {
    var ShowHeaderRows = new Array();
    ShowHeaderRows   = strHeaderRows.split(m_SplitterMarker);
    for( u=0; u < ShowHeaderRows.length; u++)
    {  
        document.getElementById('HeaderRow' + ShowHeaderRows[u]).style.display = "block";   
    }
 }



 function SetDruckHeader(strHeader) {
     document.getElementById('ele3').innerHTML = strHeader;
     //  innerHTML would not be shown example Zeugnisstelle => eugnisstelle 
     //  so open a window for forcing paint
     var winName = 'WindowsFenster';
     var winSize = 'top=2500, width=660,height=620,scrollbars=no';
     var ref = window.open('', winName, winSize);
     ref.close();
 }







 function SetHeader(strHeader) {
     
    m_ShowHeader  = new Array();
    m_ShowHeader = strHeader.split(m_SplitterMarker);
    
    if(m_CurrentHeader.indexOf("HeaderTable-1") == -1)
    {
        document.getElementById(m_CurrentHeader).style.display = "none";
    }
    
    m_CurrentHeader = 'HeaderTable' + m_ShowHeader[0];

    document.title = m_ShowHeader[1];
  
    if(m_CurrentHeader.indexOf("HeaderTable-1") == -1)
    {
        document.getElementById(m_CurrentHeader).style.display = "block";
    }

    for (u = 0; u < m_HeaderRows; u++)
    {
        document.getElementById('HeaderRow' + u).style.display = "none";   
    }

    for (v = 2; v < m_ShowHeader.length; v++)
    {
       
        if(m_ShowHeader[v].indexOf('/') > -1 && m_ShowHeader[v].indexOf('.jpg') > -1)
        {
            document.getElementById('ele' + (v - 1)).src = m_ShowHeader[v];
        }
        else
        {
            
            if (m_ShowHeader[v].indexOf('&') > -1 && m_ShowHeader[v].indexOf('&#9;') == -1) {
               
               var strHoldTheLine =  m_ShowHeader[v].split('&');
               var strStoreHoldTheLine = "";
               for (k = 0; k < strHoldTheLine.length-1; k++) {
                   strStoreHoldTheLine = strStoreHoldTheLine + strHoldTheLine[k] + '&amp;';
               }
               m_ShowHeader[v] = strStoreHoldTheLine + strHoldTheLine[strHoldTheLine.length - 1];
           }
            document.getElementById('ele' + (v - 1)).innerHTML = m_ShowHeader[v];
        }
    }
}
  
  
 
function ShowDwfSvgDrawing(ImagePath)
{
   
    m_ArrayOfAllImagePath = new Array();
    m_ArrayOfAllImagePath = ImagePath.split(m_SplitterMarker);
    var d = m_ArrayOfSVGObjects.length - 1;

    while( d > -1)//Remove all Svg Items
    {  
       var ppp = m_ArrayOfSVGObjects[d];
       document.body.removeChild(ppp);
       m_ArrayOfSVGObjects.pop();
       d--;
    }
    
 
 
    for(u = 0;  u < m_ArrayOfDWFObjects.length; u++)//Hide DWF Drawings and remove assosiated Barcode Image
    {
       var ppp = m_ArrayOfDWFObjects[u];
       if(ppp.children[m_ChildBarcode] != undefined)
       {
            ppp.removeChild(ppp.children[m_ChildBarcode]);
       }
       if(ppp.children[m_ChildBRLine] != undefined)
       {
            ppp.removeChild(ppp.children[m_ChildBRLine]);
       }
        
       ppp.style.display = "none";  
    }


   
    for( i=0; i < m_ArrayOfAllImagePath.length; i++)
    {
        var strImagePath = m_ArrayOfAllImagePath[i].split(m_SplitterMarkerjpg);
    
        if (strImagePath[0].indexOf(".svg") > -1 || strImagePath[0].indexOf(".jpg") > -1) 
        { 
           var strJpgImagePath  = m_ArrayOfAllImagePath[i].split(m_SplitterMarkerjpg);
           var embedTagSVG      = document.createElement("p");
           
           var strImagePath = strJpgImagePath[0].split(m_HochformatMarker);
        
           height = "880";
           width  = "650";
          
           if(strImagePath[1] != undefined)
           { 
              if(strImagePath[1].indexOf("0") > -1 ) // QuerFormat
              {
                 height = "440";
                 width  = "650";
              }
           }
              
           if (m_bOnlineGraficInStaticLowerSplitter.indexOf("1") > -1) //The Grafic under Tabview will be shown best zoomed in Width
           {
               
               width = "100%";
               height = "500";       
           }

           if (m_ArrayOfAllImagePath[i].indexOf(".svg") > -1)
           {

               if (m_bOnlineGraficInStaticLowerSplitter.indexOf("0") > -1) 
               {
                   try 
                   {
                       embedTagSVG.innerHTML = "<center><img src='" + strImagePath[0] + "' width='" + width + "px' height='" + height + "px' type='image/svg+xml'></img></center>";
                   }
                   catch (e) 
                   {

                       embedTagSVG.innerHTML = "<object data='" + strImagePath[0] + "' width='" + width + "px' height='" + height + "px' type='image/svg+xml'></object>";
                   }
               }
               else 
               {
                   embedTagSVG.innerHTML = "<object data='" + strImagePath[0] + "' width='" + width + "px' height='" + height + "px' type='image/svg+xml'></object>";
               }    
           }
           else 
           {
               embedTagSVG.innerHTML = "<img src='" + strImagePath[0] + "' width='" + width + "px' height='" + height + "px'/>";
           }
         
           var cell = document.body.appendChild(embedTagSVG);
           cell.align="center";
           if(m_CurrentHeader.indexOf("HeaderTable-1") == -1)
           {
            var img = document.createElement("img");
            img.align = "center";

            var strTemp = strJpgImagePath[1].split(m_SplitterCountPipes);
            img.src = strTemp[0];
           
         
            cell.appendChild(document.createElement("br"));
           
            img.height = "20";
            img.width = "100";
            cell.appendChild(img);
          
            var dic =cell.appendChild(document.createElement("p"));
            dic.style.fontSize = "large";
          
            dic.appendChild(document.createTextNode("Anzahl:  " + strTemp[1])); 
           
           }
           
           var  bFound = false;
           for( h = 0; h < m_ArrayOfDWFObjects.length; h++)
           {
                if(m_ArrayOfDWFObjects[h].style.display == "none")
                {
                   document.body.insertBefore( cell, m_ArrayOfDWFObjects[h]);
                   bFound = true;
                   break;
                }
           }
            
           if(bFound == false)
           {
               document.body.appendChild(cell);
           }
           m_ArrayOfSVGObjects.push(cell);
        }
        else
        {
            var  bFound = false;
            for( h = 0; h < m_ArrayOfDWFObjects.length; h++)
            {
                if(m_ArrayOfDWFObjects[h].style.display == "none")
                {
                    var pp = m_ArrayOfDWFObjects[h];
                    var strImagePath = m_ArrayOfAllImagePath[i].split(m_SplitterMarkerjpg);
                    DWFImage = pp.children[0];
                    DWFImage.SourcePath = strImagePath[0];
                    DWFImage.Viewer.BackColor = 255 * 65536 + 255 * 256 + 255;
                    DWFImage.Viewer.PaperVisible = false;
                    DWFImage.Viewer.ToolbarVisible = false;
                    DWFImage.width  = "650"; 
                    DWFImage.height = "880";
                    if(m_CurrentHeader.indexOf("HeaderTable-1") == -1)
                    {
                        var img = document.createElement("img");
                        img.align="center";
                        img.src = strImagePath[1];
                    
                        pp.appendChild(document.createElement("br"));
                        img.height = "20";
                        img.width = "100";
                        pp.appendChild(img);
                    }
                    m_ArrayOfDWFObjects[h].style.display = "block";
                    bFound = true;
                    break;
                }
            }
            
            if(bFound == false)
            {


                var m_DWFImage  = document.createElement ('object'); 
               
                 m_DWFImage.width  = "100%"; 
                 m_DWFImage.height = 400; 
                 m_DWFImage.setAttribute('classid', 'clsid:A662DA7E-CCB7-4743-B71A-D817F6D575DF');
                 var strImagePath = m_ArrayOfAllImagePath[i].split(m_SplitterMarkerjpg);
            
                 m_DWFImage.src = strImagePath[0]; 
                 var cell = document.createElement("p");
                 document.body.appendChild(cell);
                 cell.appendChild (m_DWFImage);
                 m_ArrayOfDWFObjects.push(cell);
                 cell.align="center";
                 if(m_CurrentHeader.indexOf("HeaderTable-1") == -1)
                 { 
                     var img = document.createElement("img");
                     img.src = strImagePath[1];
                     img.align = "center";
                     
                     img.height = "20";
                     img.width = "100";
                     cell.appendChild(document.createElement("br"));
                     cell.appendChild(img); 
                 }
              
                 var strImagePath = m_ArrayOfAllImagePath[i].split(m_SplitterMarkerjpg);
                 m_DWFImage.SourcePath = strImagePath[0];
                 m_DWFImage.Viewer.BackColor = 255 * 65536 + 255 * 256 + 255;
                 m_DWFImage.Viewer.PaperVisible = false;
                 m_DWFImage.Viewer.ToolbarVisible = false;
                 m_DWFImage.width  = "650"; 
                 m_DWFImage.height = "800";
                 scrollForPrint();
            }
        }
    }
}