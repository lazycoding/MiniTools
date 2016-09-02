//LICENSE: BSD
//Based on: https://code.google.com/p/easyexif/source/browse/trunk/exif.{h,cpp}

#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace exif
{
  enum Endian { INTEL = 0, MOTOROLA = 1 };
  enum Format {
    FORMAT_BYTE       =  1,
    FORMAT_ASCII      =  2,
    FORMAT_SHORT      =  3,
    FORMAT_LONG       =  4,
    FORMAT_RATIONAL   =  5,
    FORMAT_SBYTE      =  6,
    FORMAT_UNDEFINED  =  7,
    FORMAT_SSHORT     =  8,
    FORMAT_SLONG      =  9,
    FORMAT_SRATIONAL  = 10,
    FORMAT_FLOAT      = 11,
    FORMAT_DOUBLE     = 12
  };

  inline uint32_t to_uint32(const unsigned char buf[4], Endian endian = INTEL) {
    return endian == INTEL? 
        (uint32_t(buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0]):
        (uint32_t(buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]); 
  } 

  inline uint16_t to_uint16(const unsigned char buf[2], Endian endian = INTEL) {
    return endian == INTEL? 
        (uint16_t(buf[1] << 8) | buf[0]):
        (uint16_t(buf[0] << 8) | buf[1]);
  }

  inline double to_rational(const unsigned char buf[8], Endian endian = INTEL) {
    double numerator = (double)to_uint32(buf, endian);
    double denominator = (double)to_uint32(buf + 4, endian);
    return (denominator > 1e-20? numerator / denominator : 0.);
  }

  //NOTE: JPEG only
  inline std::string extract_exif(std::istream& is) {
    //SOI
    std::string nil;
    if (is.get() != 0xff || is.get() != 0xd8)
      return nil;

    if (is.get() != 0xff || !is.good())
      return nil;

    unsigned char c = is.get();
    //APP0
    if (c == 0xe0) {
      size_t length = (((unsigned int)is.get()) << 8) | (unsigned char)(is.get());
      if (! is.good() || length < 2) return nil;
      length -= 2;
      is.seekg(length, std::ios::cur);
      if (is.get() != 0xff || !is.good())
        return nil;
      c = is.get();
    }

    //APP1
    if (c == 0xe1) {
      size_t length = ((unsigned char)(is.get()) << 8) | (unsigned char)(is.get());
      if (! is.good()) return nil;

      std::vector<char> buf(length);
      is.read(buf.data(), length);
      if (! is.good()) return nil;

      return std::string(buf.data(), length);
    }

    return nil;
  }

  inline std::string extract_exif(const std::string& path) {
    std::ifstream ifs(path, std::ifstream::binary);
    return extract_exif(ifs);
  }

  template <typename T> struct EntryParser;
  struct Entry
  {
    uint16_t tag_;
    uint16_t format_;
    uint32_t length_;
    unsigned char data_[4];

    Endian endian_;

    Entry(const unsigned char buf[12], Endian endian = INTEL) { parse(buf, endian); }
    void parse(const unsigned char buf[12], Endian endian = INTEL) {
      tag_ = exif::to_uint16(buf, endian);
      format_ = exif::to_uint16(buf + 2, endian);
      length_ = exif::to_uint32(buf + 2 + 2, endian);
      *(uint32_t *)data_ = *(uint32_t *)(buf + 2 + 2 + 4);
      endian_ = endian;
    }

    uint8_t to_uint8() const { return data_[0]; }
    uint16_t to_uint16() const { return exif::to_uint16(data_, endian_); }
    uint32_t to_uint32() const { return exif::to_uint32(data_, endian_); }

    std::string to_string(const unsigned char * base, const unsigned char *end) const {
      if (length_ <= 4) 
        return std::string((const char *)data_, length_);

      uint32_t offset = to_uint32();
      if (base + offset + length_ <= end)
        return std::string((const char *)(base + offset), length_);

      return std::string();
    }

    double to_rational(const unsigned char *base, const unsigned char *end) const { 
      uint32_t offset = to_uint32();
      if (base + offset + 8 <= end) return exif::to_rational(base + offset, endian_);
      return 0; 
    }

    template <typename T> void parse_to(T& val, const unsigned char* base, const unsigned char *end) const {
      EntryParser<T>::parse_to(*this, val, base, end);
    };
  };

  template <> struct EntryParser<std::string> {
    static void parse_to(const Entry &entry, std::string& val, const unsigned char* base, const unsigned char *end) { val = entry.to_string(base, end); }
  };

  template <> struct EntryParser<double> {
    static void parse_to(const Entry &entry, double& val, const unsigned char* base, const unsigned char *end) { val = entry.to_rational(base, end); }
  };

  template <> struct EntryParser<unsigned int> {
    static void parse_to(const Entry &entry, unsigned int& val, const unsigned char*, const unsigned char *) { val = entry.to_uint32(); }
  };

  template <> struct EntryParser<unsigned short> {
    static void parse_to(const Entry &entry, unsigned short& val, const unsigned char*, const unsigned char *) { val = entry.to_uint16(); }
  };

  template <> struct EntryParser<char> {
    static void parse_to(const Entry &entry, char& val, const unsigned char*, const unsigned char *) { val = (char)entry.to_uint8(); }
  };

  struct Info
  {
    void reset() { *this = Info(); }
    bool parse(const std::string& buf) { return parse((const unsigned char *)buf.c_str(), buf.length()); }
    bool parse(const unsigned char *buf, size_t length) {
      size_t offset = 0;
      
      if (length < 6 + 8) {
        return false;
      }

//  6 bytes
      if (! std::equal(buf, buf + 6, "Exif\0\0")) {
        return false;
      }
      offset += 6;

//  8 bytes
//  2 bytes: 'II' or 'MM'
//  2 bytes: 0x002a
//  4 bytes: offset to first IDF
      Endian endian = INTEL;
      if (buf[offset] == 'I' && buf[offset+1] == 'I') {
        endian = INTEL;
      }
      else if (buf[offset] == 'M' && buf[offset+1] == 'M') {
        endian = MOTOROLA;
      }
      else {
        return false;
      }

      endian_ = endian;

      if (to_uint16(buf+offset + 2, endian) != 0x2a) {
        return false;
      }
      uint32_t ifd_offset = to_uint32(buf+offset + 2 + 2, endian);
      offset += ifd_offset; 

      //IFD0
      if (length < offset + 2) {
        return false;
      }
     
      int n_entries = to_uint16(buf + offset, endian);
      if (length < offset + 2 + n_entries * 12 + 4) {
        return false;
      }
      offset += 2;

      const unsigned char *base = buf + 6, *end = buf + length;
      uint32_t gps_offset = length, exif_offset = length;

#define PARSE_ENTRY(tag, val) case tag: entry.parse_to(val, base, end); break;

      while (--n_entries >= 0) {
        Entry entry(buf + offset, endian);
        offset += 12;

        switch(entry.tag_) {
        PARSE_ENTRY(0x102, BitsPerSample)
        PARSE_ENTRY(0x10E, ImageDescription)
        PARSE_ENTRY(0x10F, Make)
        PARSE_ENTRY(0x110, Model)
        PARSE_ENTRY(0x112, Orientation)
        PARSE_ENTRY(0x131, Software)
        PARSE_ENTRY(0x132, DateTime)
        PARSE_ENTRY(0x8298, Copyright)

        case 0x8825:
          // GPS IFD offset
          gps_offset = 6 + entry.to_uint32(); break;
  
        case 0x8769:
          // EXIF IFD offset
          exif_offset = 6 + entry.to_uint32(); break;

        default: break;
        }
      } // IFD0

      // EXIF
      if (exif_offset + 4 <= length) {
        offset = exif_offset;
        n_entries = to_uint16(buf + offset, endian);
        if (length < offset + 2 + n_entries * 12 + 4) {
          return false;
        }
        offset += 2;

        while (--n_entries >= 0) {
          Entry entry(buf + offset, endian);
          offset += 12;

          switch(entry.tag_) {
          PARSE_ENTRY(0x829a, ExposureTime)
          PARSE_ENTRY(0x829d, FNumber)
          PARSE_ENTRY(0x8827, ISOSpeedRatings)
          PARSE_ENTRY(0x9003, DateTimeOriginal)
          PARSE_ENTRY(0x9004, DateTimeDigitized)
          PARSE_ENTRY(0x9201, ShutterSpeedValue)
          PARSE_ENTRY(0x9202, ApertureValue)
          PARSE_ENTRY(0x9204, ExposureBiasValue)
          PARSE_ENTRY(0x9206, SubjectDistance)
          PARSE_ENTRY(0x9209, Flash)
          PARSE_ENTRY(0x920a, FocalLength)
          PARSE_ENTRY(0x9207, MeteringMode)
          PARSE_ENTRY(0x9291, SubSecTimeOriginal)
          PARSE_ENTRY(0xa405, FocalLengthIn35mm)

          case 0xa002:
            // EXIF Image width
            if (entry.format_ == FORMAT_LONG) ImageWidth = entry.to_uint32();
            else if (entry.format_ == FORMAT_SHORT) ImageWidth = entry.to_uint16();
            break;
  
          case 0xa003:
            // EXIF Image height
            if (entry.format_ == FORMAT_LONG) ImageHeight = entry.to_uint32();
            else if (entry.format_ == FORMAT_SHORT) ImageHeight = entry.to_uint16();
            break;
  
          default:
            break;
          }
        }
      } // EXIF

      // GPS
      if (gps_offset + 4 <= length) {
        offset = gps_offset;
        n_entries = to_uint16(buf + offset, endian);
        if (length < offset + 2 + n_entries * 12 + 4) {
          return false;
        }
        offset += 2;

        while (--n_entries >= 0) {
          Entry entry(buf + offset, endian);
          offset += 12;

          switch(entry.tag_) {
            PARSE_ENTRY(1, GeoLocation.LatComponents.direction)
            PARSE_ENTRY(3, GeoLocation.LonComponents.direction)
            PARSE_ENTRY(5, GeoLocation.AltitudeRef)
            PARSE_ENTRY(6, GeoLocation.Altitude)
   
            case 2:
              // GPS latitude
              if (entry.format_ == FORMAT_RATIONAL && entry.length_ == 3) {
                GeoLocation.LatComponents.degrees = entry.to_rational(base, end);
                GeoLocation.LatComponents.minutes = entry.to_rational(base + 8, end);
                GeoLocation.LatComponents.seconds = entry.to_rational(base + 16, end);
                GeoLocation.Latitude = GeoLocation.LatComponents.to_rational();
              }
              break;
   
            case 4:
              // GPS longitude
              if (entry.format_ == FORMAT_RATIONAL && entry.length_ == 3) {
                GeoLocation.LonComponents.degrees = entry.to_rational(base, end);
                GeoLocation.LonComponents.minutes = entry.to_rational(base + 8, end);
                GeoLocation.LonComponents.seconds = entry.to_rational(base + 16, end);
                GeoLocation.Longitude = GeoLocation.LonComponents.to_rational();
              }
              break;

            default:
              break;
          }
        }

        if ('S' == GeoLocation.LatComponents.direction) GeoLocation.Latitude = -GeoLocation.Latitude;
        if ('W' == GeoLocation.LonComponents.direction) GeoLocation.Longitude = -GeoLocation.Longitude;
        if (1 == GeoLocation.AltitudeRef) GeoLocation.Altitude = -GeoLocation.Altitude;
      } // GPS

      return true;
    } // parse()

    Endian endian_;

    std::string ImageDescription;     // Image description
    std::string Make;                 // Camera manufacturer's name
    std::string Model;                // Camera model
    unsigned short Orientation;       // Image orientation, start of data corresponds to
                                      // 0: unspecified in EXIF data
                                      // 1: upper left of image
                                      // 3: lower right of image
                                      // 6: upper right of image
                                      // 8: lower left of image
                                      // 9: undefined
    unsigned short BitsPerSample;     // Number of bits per component
    std::string Software;             // Software used
    std::string DateTime;             // File change date and time
    std::string SubSecTimeOriginal;   // Sub-second time that original picture was taken
    std::string Copyright;            // File copyright information

    //EXIF
    // http://www.awaresystems.be/imaging/tiff/tifftags/privateifd/exif.html
    double ExposureTime;              // Exposure time in seconds
    double FNumber;                   // F/stop
    unsigned short ExposureProgram;   // The class of the program used by the camera to set exposure when the picture is taken.
                                      // 0 = Not defined
                                      // 1 = Manual
                                      // 2 = Normal program
                                      // 3 = Aperture priority
                                      // 4 = Shutter priority
                                      // 5 = Creative program (biased toward depth of field)
                                      // 6 = Action program (biased toward fast shutter speed)
                                      // 7 = Portrait mode (for closeup photos with the background out of focus)
                                      // 8 = Landscape mode (for landscape photos with the background in focus)
//  std::string SpectralSensitivity;
    unsigned short ISOSpeedRatings;   // ISO speed
    std::string DateTimeOriginal;     // Original file date and time (may not exist)
    std::string DateTimeDigitized;    // Digitization date and time (may not exist)
    double ShutterSpeedValue;         // Shutter speed (reciprocal of exposure time)
    double ApertureValue;             // The lens aperture.
    double ExposureBiasValue;         // Exposure bias value in EV
    double MaxApertureValue;          // The smallest F number of the lens.
    double SubjectDistance;           // Distance to focus point in meters
    unsigned short MeteringMode;      // Metering mode
                                      // 1: average
                                      // 2: center weighted average
                                      // 3: spot
                                      // 4: multi-spot
                                      // 5: multi-segment
    unsigned short Flash;             // 0 = no flash, 1 = flash used
    double FocalLength;               // Focal length of lens in millimeters
    unsigned short FocalLengthIn35mm; // Focal length in 35mm film
    unsigned int ImageWidth;              // Image width reported in EXIF data
    unsigned int ImageHeight;             // Image height reported in EXIF data

    //GPS
    // http://www.awaresystems.be/imaging/tiff/tifftags/privateifd/gps.html
    struct Geolocation_t {            // GPS information embedded in file
      double Latitude;                  // Image latitude expressed as decimal
      double Longitude;                 // Image longitude expressed as decimal
      double Altitude;                  // Altitude in meters, relative to sea level
      char AltitudeRef;                 // 0 = above sea level, -1 = below sea level
      struct Coord_t {
        double degrees;               
        double minutes;
        double seconds;
        char direction;
        double to_rational() const { return degrees + minutes / 60 + seconds / 3600; }
      } LatComponents, LonComponents;   // Latitude, Longitude expressed in deg/min/sec 
    } GeoLocation; 
  };
};

