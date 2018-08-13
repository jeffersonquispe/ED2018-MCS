var min = 99;
var max = 999999;
var polygonMode = false;
var pointMode = false;
var rectMode = false;
var pointArray = new Array();
var lineArray = new Array();
var activeLine;
var activeShape = false;
var canvas;
var data1=200;
var data2=100;
var count=0;
var poligonos = [];
var polCount = 0;
var puntos = [];
var puntCount = 0;
var rect, isDown, origX, origY;//para el rectangulo

$('#k').click(function(){
        data1 = document.getElementsByName("data1")[0].value;
});


//________________Rectangulo____________________
/*$('#create-rect2').click(function(){
    var Iden=0;
    canvas.on('mouse:down', function(o){
    isDown = true;
    var pointer = canvas.getPointer(o.e);
    origX = pointer.x;
    origY = pointer.y;
    var pointer = canvas.getPointer(o.e);
    rect = new fabric.Rect({
        left: origX,
        top: origY,
        originX: 'left',
        originY: 'top',
        width: pointer.x-origX,
        height: pointer.y-origY,
        angle: 0,
        fill: 'transparent',
         strokeWidth: 2,
         strokeDashArray: [10, 5],
         stroke: 'black'
    });
    var text = new fabric.Text('B', {
        fontSize: 20,
        left: origX,
        top: origY,
        originX: 'left',
        originY: 'top',
        width: pointer.x-origX,
        height: pointer.y-origY,   
    });
    var group = new fabric.Group([ rect, text ], {
      left: origX,
      top: origY,
      id: Iden
    });
    canvas.add(group);
});*/

$('#create-rect2').click(function(){
    canvas.on('mouse:down', function(o){
    isDown = true;
    var pointer = canvas.getPointer(o.e);
    origX = pointer.x;
    origY = pointer.y;
    var pointer = canvas.getPointer(o.e);
    rect = new fabric.Rect({
        left: origX,
        top: origY,
        originX: 'left',
        originY: 'top',
        width: pointer.x-origX,
        height: pointer.y-origY,
        angle: 0,
        evented: false,
        fill: 'transparent',
         strokeWidth: 2,
         strokeDashArray: [10, 5],
         stroke: 'black'
    });
    canvas.add(rect);
        count++;
});

canvas.on('mouse:move', function(o){
    if (!isDown) return;
    var pointer = canvas.getPointer(o.e);
    if(origX>pointer.x){
        rect.set({ left: Math.abs(pointer.x) });
    }
    if(origY>pointer.y){
        rect.set({ top: Math.abs(pointer.y) });
    }
    rect.set({ width: Math.abs(origX - pointer.x) });
    rect.set({ height: Math.abs(origY - pointer.y) });
       text.id=1;
    Iden=group.get('id');
        alert(Iden);
    canvas.renderAll();
});


canvas.on('mouse:up', function(o){
  isDown = false;
});

});
//------------------------------------------------------------
$(window).load(function(){
    prototypefabric.initCanvas();
    setText();
     dibujarMBR();

    $('#create-polygon').click(function() {
        polygonMode = true;
        pointMode = false;
        prototypefabric.polygon.drawPolygon();
    });
   $('#create-point').click(function() {
        polygonMode = false;
        pointMode = true;
        prototypefabric.point.drawPoint();
      // alert("crear punto");
    });
   $('#create-rect').click(function() {
    alert("crear punto");
    prototypefabric.rect1.drawRect1(); 
        alert("crear punto2");
    });
});

var prototypefabric = new function(){
    this.initCanvas = function () {
        canvas = window._canvas = new fabric.Canvas('c');
        canvas.setWidth(800);
        canvas.setHeight(600);
        //alert($(window).width());
        //alert($(window).height()-$('#nav-bar').height());
        //canvas.selection = false;

        canvas.on('mouse:down', function (options) {
           /* alert(polygonMode);
            alert(pointMode);*/
            if(options.target && options.target.id == pointArray[0].id){
                prototypefabric.polygon.generatePolygon(pointArray);
                // -------
                // enviar el nuevo objeto a añadir
                poligonos[polCount] = prototypefabric.polygon.polygonPoints;
                polCount++;
                // -------
                prototypefabric.polygon.polygonPoints = [];
                prototypefabric.polygon.polygonLength = 0;
                alert(poligonos);
                console.log(poligonos)
            }
          if(polygonMode){
                prototypefabric.polygon.addPoint(options);
               // alert("hoal desde poly");
            }
           if(pointMode){
                prototypefabric.point.addPoint(options);
                // -------
                // enviar el nuevo objeto a añadir
                puntos[puntCount] = prototypefabric.point.points;
                puntCount++;
                // -------
                prototypefabric.point.points = [];
                console.log(puntos)
               // alert("hoal desde point");
            }
            if(rectMode){
                prototypefabric.rect.addRect(options);
               // alert("hoal desde point");
            }
        });

        canvas.on('mouse:up', function (options) {
            //alert("hola 3");

        });
        canvas.on('mouse:move', function (options) {
            if(activeLine && activeLine.class == "line"){
                var pointer = canvas.getPointer(options.e);
                activeLine.set({ x2: pointer.x, y2: pointer.y });

                var points = activeShape.get("points");
                points[pointArray.length] = {
                    x:pointer.x,
                    y:pointer.y
                }
                activeShape.set({
                    points: points
                });
                canvas.renderAll();
            }
            //alert(x);
            //------desactiva los  objetos para editarse-----
            canvas.deactivateAll();
            canvas.selection = false;
            canvas.forEachObject(function(o) {
            o.selectable = false;
             });
           //------------------------------------------------- 
            canvas.renderAll();
        });
    };
};

function deleteObjects(){
    var activeObject = canvas.getActiveObject(),
    activeGroup = canvas.getActiveGroup();
    if (activeObject) {
        if (confirm('Are you sure?')) {
            canvas.remove(activeObject);
        }
    }
    else if (activeGroup) {
        if (confirm('Are you sure?')) {
            var objectsInGroup = activeGroup.getObjects();
            canvas.discardActiveGroup();
            objectsInGroup.forEach(function(object) {
            canvas.remove(object);
            });
        }
    }
};

function resize() {
    var canvasSizer = document.getElementById("pizarra");
    var canvasScaleFactor = canvasSizer.offsetWidth/525;
    var width = canvasSizer.offsetWidth;
    var height = canvasSizer.offsetHeight;
    var ratio = canvas.getWidth() /canvas.getHeight();
         if((width/height)>ratio){
             width = height*ratio;
         } else {
             height = width / ratio;
         }
  var scale = width / canvas.getWidth();
  var zoom = canvas.getZoom();
  zoom *= scale;   
  canvas.setDimensions({ width: width, height: height });
    canvas.setViewportTransform([zoom , 0, 0, zoom , 0, 0])
}

window.addEventListener('load', resize, false);
window.addEventListener('resize', resize, false);


function dibujarMBR(){
      var obj = {data:[
        {nivel:1, tag:"R0", max:[500,500], min:[20,20]},
        {nivel:2, tag:"R1", max:[450,450], min:[300,350]},
        {nivel:2, tag:"R2", max:[300,200], min:[50,68]} ]}
        var color="blue";
        for (var i = 0; i <=2 ; i++) {
            if( i>0 && obj.data[i].nivel != obj.data[i-1].nivel){
                color=getRandomColor();
            }
            var xmax=parseInt(obj.data[i].max[0]);
            var ymax=parseInt(obj.data[i].max[1]);
            var xmin=parseInt(obj.data[i].min[0]);
            var ymin=parseInt(obj.data[i].min[1]);
            var w=xmax-xmin;var h=ymax-ymin;
            rect = new fabric.Rect({
            left: xmin,
            top: ymin,
            width: w,
            height: h,
            angle: 0,
            evented: false,
            fill: 'transparent',
             strokeWidth: 2,
             strokeDashArray: [10, 5],
             stroke: color
             });

            canvas.add(rect);

           var text = new fabric.Text(obj.data[i].tag, {
            fontSize: 15,
            evented: false,
            left: xmin,
            top: ymin
            });
           canvas.add(text);
     }
}

function getRandomColor() {
  var letters = '0123456789ABCDEF';
  var color = '#';
  for (var i = 0; i < 6; i++) {
    color += letters[Math.floor(Math.random() * 16)];
  }
  return color;
}

//function min