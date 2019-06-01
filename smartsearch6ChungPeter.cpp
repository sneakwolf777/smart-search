#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <string> 
using namespace std ;





// Find the shift value by comparing current index in text with value table
int shift_value(int * value, char * pattern, char to_find)
{
  int m = strlen(pattern) ;

  // Find matching character in pattern and return the associated value
  // in value table 
  for(int i = 0; i < m; i++)
  {
    if(pattern[i] == to_find)
      return value[i] ;
  }

  // If no match then return value associated with character in the last 
  // index position in pattern
  return m ;  
}





// Find first occurence of pattern match in text
int smart_search(char * pattern, char * text, int * value, int start, int end)
{
  int n = strlen(text) - 1 ;
  int m = strlen(pattern) - 1 ;
  int check, shift ;

  // Base case if starting position is <= last index of text
  if(start <= n)
  {
    // If last character of pattern matches
    if(pattern[m] == text[start])
    { 
      check = start ;
      // Keep checking if text from start to end indexes completely matches pattern
      do
      {
        // If pattern is only single character then return index when match found 
        if(m == 0)
	  return check ; 

	m-- ;
	check-- ;

      }while(pattern[m] == text[check] && check > end && m > 0) ;

      // If match found then return the index and exit
      if(pattern[m] == text[check])
        return check ;
      // Else no match found then shift pattern and continue
      else
      {
        shift = shift_value(value, pattern, text[start]) ;
        return smart_search(pattern, text, value, start+shift, end+shift) ;
      }
    }
    // If last character of pattern does not match then shift pattern and continue
    else
    {
        shift = shift_value(value, pattern, text[start]) ;
        return smart_search(pattern, text, value, start+shift, end+shift) ;
    } 
  }
  
  // Return False if no match found in the entire text 
  return -1 ;
}





// Check and Replace with duplicate values
void duplicate(char * pattern, int * value, int last_value)
{
  int i, j ;
  // Set current character to match with
  for(i = 0; i < last_value - 1; i++)
  { 
    // Compare current character if it matches other characters in pattern
    for(j = i+1; j <= last_value - 1; j++)
    {
      // If match found then update value table with duplicate lowest value  
      if(pattern[i] == pattern[j])
      { 
        if(value[i] > value[j])
	  value[i] = value[j] ;
	else
          value[j] = value[i] ;
      }
    }
  }
}





//Create value table
void create_value_table(char * pattern, int * value)
{ 
  int m = strlen(pattern) - 1 ;
  int last_value = m + 1 ; 
  int last = m;
  int i = 0 ;
  int j = 0 ;

  // Keep inserting decrements of m - 1 size except for last index
  while(m > 0)
  {
    value[i] = m ;
    i++ ;
    m-- ;
  }

  // Insert m value into last index
  value[last] = last_value ; 

  // If duplicate characters in pattern then update value table
  duplicate(pattern, value, last_value) ;
}





int main()
{
  int input, check, m, n, i ;
  ifstream file ;
  char temp[1000], temp2[1000] ;

  // Open text file
  file.open("text.txt") ;

  // Check if text file is not empty
  if(file)
  {
    while(!file.eof())
    {
      // Read number of text inputs in text file      
      file >> input ;
      file.ignore(1000, '\n') ;
      cout << "Number of texts to match is " << input << endl ;

      // If no more inputs then stop reading the text file
      if(input == 0)
        break ;  

      // Create a temp array for reading the pattern in text file
      file.get(temp2, 1000,'\n') ;
      file.ignore(1000, '\n') ; 

      // Create an m sized array for pattern	
      m = strlen(temp2) ;
      char pattern_array[m+1] ;
      strcpy(pattern_array, temp2) ;

      // Make sure all characters  become lowercase
      for(i = 0; i < m; i++)
      pattern_array[i] = tolower(pattern_array[i]) ;

      // Create a m sized array for value table
      int value_array[m+1] ; 

      // Insert values into value table 
      create_value_table(pattern_array, value_array) ; 

      // Keep pattern matching until no texts in file or max input is reached 
      while(input > 0)
      {
         
	// Create a temp array for reading each line in text file
	file.get(temp, 1000,'\n') ;
	file.ignore(1000, '\n') ;

	// Create a n sized array for text
        n = strlen(temp) ;
        char text_array[n+1] ;
        strcpy(text_array, temp) ;
        
        // Make sure all characters  become lowercase
        for(i = 0; i < n; i++)
          text_array[i] = tolower(text_array[i]) ;

	// Match pattern in current text
        check = smart_search(pattern_array, text_array, value_array, m - 1, 0) ;

	// If first occurence of match found then return index position
        if(check >= 0)
	  cout << "Index " << check << endl ;
	  // Else return False for no matches in text 
	else
	  cout << "False" << endl ;
        cin.ignore() ;

	// Decrement to make sure number of pattern searches matches
	// the input size as specificed by user
        input-- ;
      }
    
    } 
  }
  // Close text file and exit program 
  file.close() ;
  return 0 ; 
}
