/* ************************************************************************** */
/** DB_Debounce

  @Company
 Torka AS

  @File Name
    DB_Debounce.h

  @Summary
    A simple three samples in a row debounce/ hysteresis filter.

  @Description
    A simple three samples in a row debounce/ hysteresis filter.
 */
/* ************************************************************************** */

#ifndef _DB_DEBOUNCE_H    /* Guard against multiple inclusion */
#define _DB_DEBOUNCE_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */


    
    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    typedef struct _DB_debounce_struct_t {
        bool v3;
        bool v2;
        bool v1;
        bool value;
        bool handled;
    } DB_debounce_struct_t;

    

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************


    // *****************************************************************************
    /**
      @Function
        int ExampleFunctionName ( int param1, int param2 ) 

      @Summary
        Brief one-line description of the function.

      @Description
        Full description, explaining the purpose and usage of the function.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Precondition
        List and describe any required preconditions. If there are no preconditions,
        enter "None."

      @Parameters
        @param param1 Describe the first parameter to the function.
    
        @param param2 Describe the second parameter to the function.

      @Returns
        List (if feasible) and describe the return values of the function.
        <ul>
          <li>1   Indicates an error occurred
          <li>0   Indicates an error did not occur
        </ul>

      @Remarks
        Describe any special behavior not described above.
        <p>
        Any additional remarks.

      @Example
        @code
        if(ExampleFunctionName(1, 2) == 0)
        {
            return 3;
        }
     */
    bool DB_DebounceSignal(bool newSignal, DB_debounce_struct_t * param);

    

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
