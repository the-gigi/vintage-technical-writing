/*
 * Controller.java
 *
 * Created on September 3, 2005, 4:13 PM
 *
 * To change this template, choose Tools | Options and locate the template under
 * the Source Creation and Management node. Right-click the template and choose
 * Open. You can then make changes to the template in the Source Editor.
 */

package com.GigiZone.SudokuServer;
import com.GigiZone.Sudoku.Board;

import org.springframework.web.servlet.ModelAndView;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.web.servlet.mvc.SimpleFormController;
import org.springframework.validation.BindException;

/**
 *
 * @author Gigi
 */
public class SudokuServerController extends SimpleFormController
{  
    protected final Log logger = LogFactory.getLog(getClass());
    
    public Object formBackingObject(HttpServletRequest request) throws ServletException
    {
        this.setSessionForm(true);

        logger.info("SudokuServerController.fromBackingObject() - returning inital board");
     
//        String s = "357862941" +
//                   "162974853" +
//                   "489351267" +
//                   "816527439" +
//                   "593148726" +
//                   "724693185" +
//                   "238715694" +
//                   "941236578" +
//                   "075489312";
        
        String s = "350060000" +
                   "002970800" +
                   "000001000" +
                   "800020009" +
                   "590000026" +
                   "700090005" +
                   "000700000" +
                   "001036500" +
                   "000080012";
        
        Board board = new Board(s);
        board.setName("Yeahh, it works!!!");
        
        return board;
        
    }
             
    protected ModelAndView onSubmit(HttpServletRequest request,
                                      HttpServletResponse response, 
                                      Object command,	
                                      BindException errors) throws Exception
    {
        Board board = (Board)command; 
        String boardString = request.getParameter("boardString");
        board.assignString(boardString);
        
        if (!board.validate())
            errors.reject("error"); // "error" is the error code in WEB-INF\classes\messages.properties        
        
        return showForm(request, errors, "sudoku", errors.getModel());
    }
}
