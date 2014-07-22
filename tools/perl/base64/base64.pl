#!/usr/bin/perl  
      
use MIME::Base64;  
  
printf("orignal string is 'Man'\n");  
$encoded = encode_base64('Man');  
printf("after base64 encoded is: %s", $encoded);  
$decoded = decode_base64($encoded);  
printf("after base64 decoded is: %s\n\n", $decoded);  
  
  
printf("\nencode_base64\n");  
  
# YTph  
$encoded1 = encode_base64('a:a');  
print $encoded1;  
  
# YTphYQ==  
$encoded2 = encode_base64('a:aa');  
print $encoded2;  
  
# YTphYWE=  
$encoded3 = encode_base64('a:aaa');  
print $encoded3;  
  
# YTphYWFh  
$encoded4 = encode_base64('a:aaaa');  
print $encoded4;  
  
printf("\ndecode_base64\n");  
  
$decoded1 = decode_base64($encoded1);  
print $decoded1 ."\n";  
  
$decoded2 = decode_base64($encoded2);  
print $decoded2 ."\n";  
  
$decoded3 = decode_base64($encoded3);  
print $decoded3 ."\n";  
  
$decoded4 = decode_base64($encoded4);  
print $decoded4 ."\n";  
  
system(pause);  


