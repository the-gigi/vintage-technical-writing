<%@ include file="/WEB-INF/jspf/include.jspf" %>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags" %>

<%@page contentType="text/html"%>
<%@page pageEncoding="UTF-8"%>

<%@page import="java.util.Map" %>
<%@page import="com.GigiZone.Sudoku.Board" %>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title><fmt:message key="title"/></title>
        <link rel="stylesheet" type="text/css" href="sudoku.css" media="screen" />
        <script language="JavaScript">
        <!--
            function getObject(name)
            {
                if (document.getElementById)
                    return document.getElementById(name);               
                else if (document.all)
                    return document.all[name];                
                else if (document.layers)                
                    return document.layers[name];
                else
                    return null;
            }        
            
            function onKeyDown(event) 
            {                
                var e = event ? event : window.event;
                
                var code;
                if (e.keyCode)
                    code = e.keyCode;
                else if (e.which) 
                    code = e.which;
                                                   
                var enable = true;
                if (e.shiftKey || e.ctrlKey || e.altKey)
                    enable = false;
                
                
                if (!((code > 48 && code < 58) || code == 46 || code == 8))
                    enable = false;

                // Left/Right arrow keys are allowed with Shift for selection
                if (e.shiftKey && (code == 37 || code == 39))
                    enable = true;
                    
                if (enable)
                    return;
                
                e.returnValue = false;                
                e.cancelBubble = true;
                
                if (document.all)
                {                
                    e.keyCode = 0;
                }
                else
                {                    
                    e.preventDefault();
                    e.stopPropagation();
                }                    
            }
            
            function onSubmit() 
            {
                b = getObject("boardTable");
                var i, j, row, id, cell, rows;
                
                rows = '';
                for (i = 1; i < 10; ++i)
                {
                    row = '';
                    for (j = 1; j < 10; ++j)
                    {
                        id = 'c' + i + j;
                        cell = getObject(id);
                        var value = '0';
                        if (cell.value != '')
                            value = cell.value;
                        row += value;
                    }
                    rows += row;                    
                }
                
                //alert(rows);
                var s = getObject("boardString");
                s.value = rows;
                return true;            
            }
        //-->
        </script>        
    </head>
<body>
    <h1><fmt:message key="heading"/></h1>  
        <table cellspacing="0px" cellpadding="8px" id="boardTable">
        <% // Get the Items from the request stream
            Board board = (Board)request.getAttribute("solution");
            String[][] cellStyles = 
            { 
                { "tl-cell", "tm-cell", "tm-cell",
                  "tl-cell", "tm-cell", "tm-cell",
                  "tl-cell", "tm-cell", "tm-cell" },

                {   "ml-cell", "mm-cell", "mm-cell",
                    "ml-cell", "mm-cell", "mm-cell",
                    "ml-cell", "mm-cell", "mm-cell" },

                {   "ml-cell", "mm-cell", "mm-cell",
                    "ml-cell", "mm-cell", "mm-cell",
                    "ml-cell", "mm-cell", "mm-cell" }
            };

            out.print("    ");
            for (int i = 0; i < 9; ++i)
            {
                out.println("<tr>");
                for (int j = 0; j <9; ++j)
                {
                    String cellId = "c" + new Integer(i+1).toString() + new Integer(j+1).toString();
                    String id = "id='" + cellId + "'";
                    String name = "name='" + cellId + "'";
                    String readonly = "";
                    String inputStyle = "class='writeable-cell'";
                    String value = "";

                    if (board.getInitialSquare(i,j) != 0)
                    {
                        readonly = "readonly='1'";
                        inputStyle = "class='readonly-cell'";
                    }

                    if (board.getSquare(i,j) != 0)
                        value = new Integer(board.getSquare(i,j)).toString();

                    String input = "<input onkeydown='onKeyDown(event)' onkeypress='onKeyDown(event)' maxlength='1' " + readonly + " " + 
                            inputStyle + " " + id + " " + name + "' value='" + value + "' />";
                    String cellStyle = cellStyles[i%3][j%3];
                    String cell = "<td class=" + cellStyle + ">" + input + "</td>";                        
                    out.print("                    ");
                    out.println(cell);
                }
                out.println("                </tr>");
                out.print("                ");
            }
        %>
        </table>
        <form id="board" method="POST" action="sudoku.html" onSubmit="onSubmit()">
            <div>&nbsp;</div>
            <spring:bind path="solution.string">
                <input type="hidden" value="" id="boardString" name="boardString" />
            </spring:bind>
            <c:if test="${solution.done}">
                <input type="submit" alignment="left" value="Submit" disabled="disabled" />
            </c:if>
            <c:if test="${!solution.done}">
                <input type="submit" alignment="left" value="Submit" />
            </c:if>           
        </form>
        <div>
            <c:choose> 
               <c:when test="${solution.valid}">
                    <c:choose> 
                       <c:when test="${solution.done}">
                            <fmt:message key="done"/>
                       </c:when>
                       <c:otherwise>
                            <fmt:message key="ok"/>
                       </c:otherwise>                       
                    </c:choose>        
               </c:when>
               <c:otherwise>
                    <fmt:message key="error"/>
               </c:otherwise>
            </c:choose>        
        </div>             
        
        <!--
        <c:if test="${!solution.valid}">
            <div>The board contains some errors</div>
        </c:if>
        <c:if test="${solution.done}">
            <div>Congratulations !!!</div>
        </c:if>
        -->
               
</body>        
</html>
