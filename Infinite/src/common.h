#pragma once

#define SAFE_DELETE(ptr) { if(ptr){ delete ptr; ptr = nullptr; } }