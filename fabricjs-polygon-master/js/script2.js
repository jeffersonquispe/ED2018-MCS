var min = 99;
var max = 999999;
var polygonMode = true;
var pointArray = new Array();
var lineArray = new Array();
var activeLine;
var activeShape = false;
var canvas
$(window).load(function(){
    prototypefabric.initCanvas();
    $('#create-points').click(function() {
        prototypefabric.polygon.drawPolygon(); 
        //alert("hola point");
    });
});
var prototypefabric = new function () {
    this.initCanvas = function () {

        canvas = window._canvas = new fabric.Canvas('c');
        canvas.setWidth(800);
        canvas.setHeight(600);
        //alert($(window).width());
        //alert($(window).height()-$('#nav-bar').height());
        //canvas.selection = false;

        canvas.on('mouse:down', function (options) {
            if(options.target && options.target.id == pointArray[0].id){
                prototypefabric.polygon.generatePolygon(pointArray);
            }
            if(polygonMode){
                prototypefabric.polygon.addPoint(options);
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
            canvas.renderAll();
        });
    };
};


,
    addRect : function(options) {
      var rect = new fabric.Rect({
      left: 100,
      top: 100,
      width: 100,
      height: 100,
      fill:'transparent',
      selection: false,
      strokeWidth: 2,
      strokeDashArray: [10, 5],
      stroke: 'green'
    });
    pointArray.push(rect);
    var text = new fabric.Text('B', {
      fontSize: 20,
       left: 110,
        top: 100,    
    });

    var group = new fabric.Group([ rect, text ], {
      left: 150,
      top: 100,
    });

    canvas.add(group);
     canvas.renderAll();
    },

     limpiarpoint : function() {
        circle.length=0;
      //  alert("limpiar punto");
    }