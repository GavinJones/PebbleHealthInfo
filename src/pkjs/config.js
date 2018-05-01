module.exports = [
  { 
    "type": "heading", 
    "defaultValue": "Health Viewer" 
  }, 
  { 
    "type": "text", 
    "defaultValue": "By Gavin Jones." 
  },
  { "type": "section",
    "items": [
    {
      "type": "heading",
      "defaultValue": "General"
    },  
    {
     "type": "slider",
     "messageKey": "stepgoal",
     "defaultValue": 8000,
     "label": "Step Goal",
     "description": "Choose required step goal",
     "min": 2000,
     "max": 20000,
     "step": 1000
   },
   {
     "type": "color",
     "messageKey": "stepcolour",
     "defaultValue": "0000ff",
     "label": "Step Text Colour",
     "sunlight": false,
     "allowGray": true
   },      
   ]
},   
{
  "type": "submit",
  "defaultValue": "Save"
}  
  
];


