// Taste The Code - Blagojce Bill Kolicoski
// Google Sheets controlled Arduino smart light project
// https://youtu.be/aKgtz3-gwrY

// Built by an example script from: https://github.com/electronicsguy


var SS = SpreadsheetApp.openById('1-8Tn9usMtXkHwacRplh1rRdV8QBK4vw-qGxwyCIXaGc');
var sheet = SS.getSheetByName('Sheet1');

function doGet(e){
  
  var read = e.parameter.read;
  
  if (read !== undefined){
    return ContentService.createTextOutput(sheet.getRange(read).getValue());
  }
  
  return ContentService.createTextOutput("No value passed as argument to script Url.");
}