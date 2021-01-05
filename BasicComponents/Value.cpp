#include "Value.hpp"
#include <sstream>
#include <iomanip>




const ValueVector ValueVectorNull;
const ValueMap ValueMapNull;
const ValueMapIntKey ValueMapIntKeyNull;

const Value Value::Null;

Value::Value()
: _type(TypeX::NONE)
{
  memset(&_field, 0, sizeof(_field));
}

Value::Value(unsigned char v)
: _type(TypeX::BYTE)
{
  _field.byteVal = v;
}

Value::Value(int v)
: _type(TypeX::INTEGER)
{
  _field.intVal = v;
}

Value::Value(float v)
: _type(TypeX::FLOAT)
{
  _field.floatVal = v;
}

Value::Value(double v)
: _type(TypeX::DOUBLE)
{
  _field.doubleVal = v;
}

Value::Value(bool v)
: _type(TypeX::BOOLEAN)
{
  _field.boolVal = v;
}

Value::Value(const char* v)
: _type(TypeX::STRING)
{
  _field.strVal = new std::string();
  if (v)
  {
      *_field.strVal = v;
  }
}

Value::Value(const std::string& v)
: _type(TypeX::STRING)
{
  _field.strVal = new std::string();
  *_field.strVal = v;
}

Value::Value(const ValueVector& v)
: _type(TypeX::VECTOR)
{
  _field.vectorVal = new (std::nothrow) ValueVector();
  *_field.vectorVal = v;
}

Value::Value(ValueVector&& v)
: _type(TypeX::VECTOR)
{
  _field.vectorVal = new (std::nothrow) ValueVector();
  *_field.vectorVal = std::move(v);
}

Value::Value(const ValueMap& v)
: _type(TypeX::MAP)
{
  _field.mapVal = new (std::nothrow) ValueMap();
  *_field.mapVal = v;
}

Value::Value(ValueMap&& v)
: _type(TypeX::MAP)
{
  _field.mapVal = new (std::nothrow) ValueMap();
  *_field.mapVal = std::move(v);
}

Value::Value(const ValueMapIntKey& v)
: _type(TypeX::INT_KEY_MAP)
{
  _field.intKeyMapVal = new (std::nothrow) ValueMapIntKey();
  *_field.intKeyMapVal = v;
}

Value::Value(ValueMapIntKey&& v)
: _type(TypeX::INT_KEY_MAP)
{
  _field.intKeyMapVal = new (std::nothrow) ValueMapIntKey();
  *_field.intKeyMapVal = std::move(v);
}

Value::Value(const Value& other)
: _type(TypeX::NONE)
{
  *this = other;
}

Value::Value(Value&& other)
: _type(TypeX::NONE)
{
  *this = std::move(other);
}

Value::~Value()
{
  clear();
}

Value& Value::operator= (const Value& other)
{
  if (this != &other) {
      reset(other._type);

      switch (other._type) {
          case TypeX::BYTE:
              _field.byteVal = other._field.byteVal;
              break;
          case TypeX::INTEGER:
              _field.intVal = other._field.intVal;
              break;
          case TypeX::FLOAT:
              _field.floatVal = other._field.floatVal;
              break;
          case TypeX::DOUBLE:
              _field.doubleVal = other._field.doubleVal;
              break;
          case TypeX::BOOLEAN:
              _field.boolVal = other._field.boolVal;
              break;
          case TypeX::STRING:
              if (_field.strVal == nullptr)
              {
                  _field.strVal = new std::string();
              }
              *_field.strVal = *other._field.strVal;
              break;
          case TypeX::VECTOR:
              if (_field.vectorVal == nullptr)
              {
                  _field.vectorVal = new (std::nothrow) ValueVector();
              }
              *_field.vectorVal = *other._field.vectorVal;
              break;
          case TypeX::MAP:
              if (_field.mapVal == nullptr)
              {
                  _field.mapVal = new (std::nothrow) ValueMap();
              }
              *_field.mapVal = *other._field.mapVal;
              break;
          case TypeX::INT_KEY_MAP:
              if (_field.intKeyMapVal == nullptr)
              {
                  _field.intKeyMapVal = new (std::nothrow) ValueMapIntKey();
              }
              *_field.intKeyMapVal = *other._field.intKeyMapVal;
              break;
          default:
              break;
      }
  }
  return *this;
}

Value& Value::operator= (Value&& other)
{
  if (this != &other)
  {
      clear();
      switch (other._type)
      {
          case TypeX::BYTE:
              _field.byteVal = other._field.byteVal;
              break;
          case TypeX::INTEGER:
              _field.intVal = other._field.intVal;
              break;
          case TypeX::FLOAT:
              _field.floatVal = other._field.floatVal;
              break;
          case TypeX::DOUBLE:
              _field.doubleVal = other._field.doubleVal;
              break;
          case TypeX::BOOLEAN:
              _field.boolVal = other._field.boolVal;
              break;
          case TypeX::STRING:
              _field.strVal = other._field.strVal;
              break;
          case TypeX::VECTOR:
              _field.vectorVal = other._field.vectorVal;
              break;
          case TypeX::MAP:
              _field.mapVal = other._field.mapVal;
              break;
          case TypeX::INT_KEY_MAP:
              _field.intKeyMapVal = other._field.intKeyMapVal;
              break;
          default:
              break;
      }
      _type = other._type;

      memset(&other._field, 0, sizeof(other._field));
      other._type = TypeX::NONE;
  }

  return *this;
}

Value& Value::operator= (unsigned char v)
{
  reset(TypeX::BYTE);
  _field.byteVal = v;
  return *this;
}

Value& Value::operator= (int v)
{
  reset(TypeX::INTEGER);
  _field.intVal = v;
  return *this;
}

Value& Value::operator= (float v)
{
  reset(TypeX::FLOAT);
  _field.floatVal = v;
  return *this;
}

Value& Value::operator= (double v)
{
  reset(TypeX::DOUBLE);
  _field.doubleVal = v;
  return *this;
}

Value& Value::operator= (bool v)
{
  reset(TypeX::BOOLEAN);
  _field.boolVal = v;
  return *this;
}

Value& Value::operator= (const char* v)
{
  reset(TypeX::STRING);
  *_field.strVal = v ? v : "";
  return *this;
}

Value& Value::operator= (const std::string& v)
{
  reset(TypeX::STRING);
  *_field.strVal = v;
  return *this;
}

Value& Value::operator= (const ValueVector& v)
{
  reset(TypeX::VECTOR);
  *_field.vectorVal = v;
  return *this;
}

Value& Value::operator= (ValueVector&& v)
{
  reset(TypeX::VECTOR);
  *_field.vectorVal = std::move(v);
  return *this;
}

Value& Value::operator= (const ValueMap& v)
{
  reset(TypeX::MAP);
  *_field.mapVal = v;
  return *this;
}

Value& Value::operator= (ValueMap&& v)
{
  reset(TypeX::MAP);
  *_field.mapVal = std::move(v);
  return *this;
}

Value& Value::operator= (const ValueMapIntKey& v)
{
  reset(TypeX::INT_KEY_MAP);
  *_field.intKeyMapVal = v;
  return *this;
}

Value& Value::operator= (ValueMapIntKey&& v)
{
  reset(TypeX::INT_KEY_MAP);
  *_field.intKeyMapVal = std::move(v);
  return *this;
}

bool Value::operator!= (const Value& v)
{
  return !(*this == v);
}
bool Value::operator!= (const Value& v) const
{
  return !(*this == v);
}

bool Value::operator== (const Value& v)
{
  const auto &t = *this;
  return t == v;
}
bool Value::operator== (const Value& v) const
{
  if (this == &v) return true;
  if (v._type != this->_type) return false;
  if (this->isNull()) return true;
  switch (_type)
  {
  case TypeX::BYTE:    return v._field.byteVal   == this->_field.byteVal;
  case TypeX::INTEGER: return v._field.intVal    == this->_field.intVal;
  case TypeX::BOOLEAN: return v._field.boolVal   == this->_field.boolVal;
  case TypeX::STRING:  return *v._field.strVal   == *this->_field.strVal;
  case TypeX::FLOAT:   return fabs(v._field.floatVal  - this->_field.floatVal)  <= 0.00001f;
  case TypeX::DOUBLE:  return fabs(v._field.doubleVal - this->_field.doubleVal) <= 0.00001f;
  case TypeX::VECTOR:
  {
      const auto &v1 = *(this->_field.vectorVal);
      const auto &v2 = *(v._field.vectorVal);
      const auto size = v1.size();
      if (size == v2.size())
      {
          for (size_t i = 0; i < size; i++)
          {
              if (v1[i] != v2[i]) return false;
          }
      }
      return true;
  }
  case TypeX::MAP:
  {
      const auto &map1 = *(this->_field.mapVal);
      const auto &map2 = *(v._field.mapVal);
      for (const auto &kvp : map1)
      {
          auto it = map2.find(kvp.first);
          if (it == map2.end() || it->second != kvp.second)
          {
              return false;
          }
      }
      return true;
  }
  case TypeX::INT_KEY_MAP:
  {
      const auto &map1 = *(this->_field.intKeyMapVal);
      const auto &map2 = *(v._field.intKeyMapVal);
      for (const auto &kvp : map1)
      {
          auto it = map2.find(kvp.first);
          if (it == map2.end() || it->second != kvp.second)
          {
              return false;
          }
      }
      return true;
  }
  default:
      break;
  };

  return false;
}

/// Convert value to a specified type
unsigned char Value::asByte() const
{


  if (_type == TypeX::BYTE)
  {
      return _field.byteVal;
  }

  if (_type == TypeX::INTEGER)
  {
      return static_cast<unsigned char>(_field.intVal);
  }

  if (_type == TypeX::STRING)
  {
      return static_cast<unsigned char>(atoi(_field.strVal->c_str()));
  }

  if (_type == TypeX::FLOAT)
  {
      return static_cast<unsigned char>(_field.floatVal);
  }

  if (_type == TypeX::DOUBLE)
  {
      return static_cast<unsigned char>(_field.doubleVal);
  }

  if (_type == TypeX::BOOLEAN)
  {
      return _field.boolVal ? 1 : 0;
  }

  return 0;
}

int Value::asInt() const
{

  if (_type == TypeX::INTEGER)
  {
      return _field.intVal;
  }

  if (_type == TypeX::BYTE)
  {
      return _field.byteVal;
  }

  if (_type == TypeX::STRING)
  {
      return atoi(_field.strVal->c_str());
  }

  if (_type == TypeX::FLOAT)
  {
      return static_cast<int>(_field.floatVal);
  }

  if (_type == TypeX::DOUBLE)
  {
      return static_cast<int>(_field.doubleVal);
  }

  if (_type == TypeX::BOOLEAN)
  {
      return _field.boolVal ? 1 : 0;
  }

  return 0;
}

float Value::asFloat() const
{

  if (_type == TypeX::FLOAT)
  {
      return _field.floatVal;
  }

  if (_type == TypeX::BYTE)
  {
      return static_cast<float>(_field.byteVal);
  }

  if (_type == TypeX::STRING)
  {
      return atof(_field.strVal->c_str());
  }

  if (_type == TypeX::INTEGER)
  {
      return static_cast<float>(_field.intVal);
  }

  if (_type == TypeX::DOUBLE)
  {
      return static_cast<float>(_field.doubleVal);
  }

  if (_type == TypeX::BOOLEAN)
  {
      return _field.boolVal ? 1.0f : 0.0f;
  }

  return 0.0f;
}

double Value::asDouble() const
{

  if (_type == TypeX::DOUBLE)
  {
      return _field.doubleVal;
  }

  if (_type == TypeX::BYTE)
  {
      return static_cast<double>(_field.byteVal);
  }

  if (_type == TypeX::STRING)
  {
      return static_cast<double>(atof(_field.strVal->c_str()));
  }

  if (_type == TypeX::INTEGER)
  {
      return static_cast<double>(_field.intVal);
  }

  if (_type == TypeX::FLOAT)
  {
      return static_cast<double>(_field.floatVal);
  }

  if (_type == TypeX::BOOLEAN)
  {
      return _field.boolVal ? 1.0 : 0.0;
  }

  return 0.0;
}

bool Value::asBool() const
{

  if (_type == TypeX::BOOLEAN)
  {
      return _field.boolVal;
  }

  if (_type == TypeX::BYTE)
  {
      return _field.byteVal == 0 ? false : true;
  }

  if (_type == TypeX::STRING)
  {
      return (*_field.strVal == "0" || *_field.strVal == "false") ? false : true;
  }

  if (_type == TypeX::INTEGER)
  {
      return _field.intVal == 0 ? false : true;
  }

  if (_type == TypeX::FLOAT)
  {
      return _field.floatVal == 0.0f ? false : true;
  }

  if (_type == TypeX::DOUBLE)
  {
      return _field.doubleVal == 0.0 ? false : true;
  }

  return false;
}

std::string Value::asString() const
{


  if (_type == TypeX::STRING)
  {
      return *_field.strVal;
  }

  std::stringstream ret;

  switch (_type)
  {
      case TypeX::BYTE:
          ret << _field.byteVal;
          break;
      case TypeX::INTEGER:
          ret << _field.intVal;
          break;
      case TypeX::FLOAT:
          ret << std::fixed << std::setprecision( 7 )<< _field.floatVal;
          break;
      case TypeX::DOUBLE:
          ret << std::fixed << std::setprecision( 16 ) << _field.doubleVal;
          break;
      case TypeX::BOOLEAN:
          ret << (_field.boolVal ? "true" : "false");
          break;
      default:
          break;
  }
  return ret.str();
}

ValueVector& Value::asValueVector()
{

  return *_field.vectorVal;
}

const ValueVector& Value::asValueVector() const
{

  return *_field.vectorVal;
}

ValueMap& Value::asValueMap()
{

  return *_field.mapVal;
}

const ValueMap& Value::asValueMap() const
{

  return *_field.mapVal;
}

ValueMapIntKey& Value::asIntKeyMap()
{

  return *_field.intKeyMapVal;
}

const ValueMapIntKey& Value::asIntKeyMap() const
{
      return *_field.intKeyMapVal;
}

static std::string getTabs(int depth)
{
  std::string tabWidth;

  for (int i = 0; i < depth; ++i)
  {
      tabWidth += "\t";
  }

  return tabWidth;
}

static std::string visit(const Value& v, int depth);

static std::string visitVector(const ValueVector& v, int depth)
{
  std::stringstream ret;

  if (depth > 0)
      ret << "\n";

  ret << getTabs(depth) << "[\n";

  int i = 0;
  for (const auto& child : v)
  {
      ret << getTabs(depth+1) << i << ": " << visit(child, depth + 1);
      ++i;
  }

  ret << getTabs(depth) << "]\n";

  return ret.str();
}

template <class T>
static std::string visitMap(const T& v, int depth)
{
  std::stringstream ret;

  if (depth > 0)
      ret << "\n";

  ret << getTabs(depth) << "{\n";

  for (auto iter = v.begin(); iter != v.end(); ++iter)
  {
      ret << getTabs(depth + 1) << iter->first << ": ";
      ret << visit(iter->second, depth + 1);
  }

  ret << getTabs(depth) << "}\n";

  return ret.str();
}

static std::string visit(const Value& v, int depth)
{
  std::stringstream ret;

  switch (v.getType())
  {
      case Value::TypeX::NONE:
      case Value::TypeX::BYTE:
      case Value::TypeX::INTEGER:
      case Value::TypeX::FLOAT:
      case Value::TypeX::DOUBLE:
      case Value::TypeX::BOOLEAN:
      case Value::TypeX::STRING:
          ret << v.asString() << "\n";
          break;
      case Value::TypeX::VECTOR:
          ret << visitVector(v.asValueVector(), depth);
          break;
      case Value::TypeX::MAP:
          ret << visitMap(v.asValueMap(), depth);
          break;
      case Value::TypeX::INT_KEY_MAP:
          ret << visitMap(v.asIntKeyMap(), depth);
          break;
      default:

          break;
  }

  return ret.str();
}

std::string Value::getDescription() const
{
  std::string ret("\n");
  ret += visit(*this, 0);
  return ret;
}

void Value::clear()
{
  // Free memory the old value allocated
  switch (_type)
  {
      case TypeX::BYTE:
          _field.byteVal = 0;
          break;
      case TypeX::INTEGER:
          _field.intVal = 0;
          break;
      case TypeX::FLOAT:
          _field.floatVal = 0.0f;
          break;
      case TypeX::DOUBLE:
          _field.doubleVal = 0.0;
          break;
      case TypeX::BOOLEAN:
          _field.boolVal = false;
          break;
              default:
          break;
  }

  _type = TypeX::NONE;
}

void Value::reset(TypeX type)
{
  if (_type == type)
      return;

  clear();

  // Allocate memory for the new value
  switch (type)
  {
      case TypeX::STRING:
          _field.strVal = new std::string();
          break;
      case TypeX::VECTOR:
          _field.vectorVal = new (std::nothrow) ValueVector();
          break;
      case TypeX::MAP:
          _field.mapVal = new (std::nothrow) ValueMap();
          break;
      case TypeX::INT_KEY_MAP:
          _field.intKeyMapVal = new (std::nothrow) ValueMapIntKey();
          break;
      default:
          break;
  }

  _type = type;
}

