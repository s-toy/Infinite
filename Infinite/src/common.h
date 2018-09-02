#pragma once

#define SAFE_DELETE(ptr) { if (ptr) { delete ptr; ptr = nullptr; } }
#define SAFE_DELETE_ARRAY(ptr) { if (ptr) { delete[] ptr; ptr = nullptr; } }