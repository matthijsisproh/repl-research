#line 1 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-Azure\\src\\iotc\\common\\string_buffer.h"
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license.

#ifndef AZURE_IOTC_LITE_STRING_BUFFER_H
#define AZURE_IOTC_LITE_STRING_BUFFER_H

namespace AzureIOT {

class StringBuffer {
  char* data;
  const char* immutable;
  unsigned length;

 public:
  StringBuffer() : data(NULL), immutable(NULL), length(0) {}

  StringBuffer(StringBuffer& buffer);

  StringBuffer(const char* str, unsigned int lengthStr, bool isCopy = true);

  StringBuffer(unsigned lengthStr);

  void initialize(const char* str, unsigned lengthStr);
  void alloc(unsigned lengthStr);
  void set(unsigned index, char c);
  void clear();
  ~StringBuffer();

  char* operator*() { return data; }
  unsigned getLength() { return length; }
  void setLength(unsigned l);
  bool startsWith(const char* str, size_t len);
  int32_t indexOf(const char* look_for, size_t look_for_length,
                  int32_t start_index = 0);

#if defined(__MBED__) || defined(ARDUINO)
  bool hash(const char* key, unsigned key_length);
#endif

  bool urlDecode();
  bool urlEncode();

  bool base64Decode();
  bool base64Encode();
};

}  // namespace AzureIOT

#endif  // AZURE_IOTC_LITE_STRING_BUFFER_H