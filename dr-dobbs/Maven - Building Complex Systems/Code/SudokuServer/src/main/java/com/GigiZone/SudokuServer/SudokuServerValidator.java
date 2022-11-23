package com.GigiZone.SudokuServer;

import com.GigiZone.Sudoku.Board;
import org.springframework.validation.Validator;
import org.springframework.validation.Errors;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class SudokuServerValidator implements Validator 
{
    protected final Log logger = LogFactory.getLog(getClass());

    public boolean supports(Class clazz) 
    {
        return clazz.equals(Board.class);
    }

    public void validate(Object obj, Errors errors) 
    {        
        Board b = (Board)obj;
        int s00 = b.getSquare(0,0);
        logger.info("SudokuServerValidator.validate() is here..., s00=" + new Integer(s00).toString());
        
        String s = b.getString();
        if (s == null)
            return;
        
        Board newBoard = new Board(s);
        if (!newBoard.validate())
            errors.reject("error"); // "error" is the error code in WEB-INF\classes\messages.properties
        else 
            b.assignString(s);
    }    
}
