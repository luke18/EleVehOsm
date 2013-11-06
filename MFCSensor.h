/*
Module : MFCSensor.H
Purpose: Defines the interface for a set of MFC wrapper classes for the Windows 7 Sensors API
Created: PJN / 05-02-2011
History: PJN / 07-02-2011 1. CMFCLocationSensorDataReport is now derived from a new "CMFCSensorDataReport" class.
                          2. Provided a number of new SensorDataReport classes including CMFCElectricalSensorDataReport,
                          CMFCEnvironmentalSensorDataReport, CMFCLightSensorDataReport, CMFCMechanicalSensorDataReport,
                          CMFCMotionSensorDataReport, CMFCOrientationSensorDataReport & CMFCScannerSensorDataReport

Copyright (c) 2011 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


////////////////////////// Macros / Defines ///////////////////////////////////

#pragma once

#ifndef __MFCSENSOR_H__
#define __MFCSENSOR_H__


///////////////////////// Includes ////////////////////////////////////////////

#ifndef __sensorsapi_h__
#pragma message("To avoid this message, please put sensorsapi.h in your pre compiled header (normally stdafx.h)")
#include <sensorsapi.h>
#endif

#ifndef _SENSORS_H_
#pragma message("To avoid this message, please put sensors.h in your pre compiled header (normally stdafx.h)")
#include <sensors.h>
#endif

#ifndef __ATLCOM_H__
#pragma message("To avoid this message, please put atlcom.h in your pre compiled header (normally stdafx.h)")
#include <atlcom.h>
#endif

#ifndef __AFXTEMPL_H__
#pragma message("To avoid this message, please put afxtempl.h in your pre compiled header (normally stdafx.h)")
#include <afxtempl.h>
#endif

#pragma comment(lib, "sensorsapi.lib")  //Automatically link to the Sensor API library


///////////////////////// Classes /////////////////////////////////////////////

//The main wrapper for the ISensorManagerEvents interface
class ATL_NO_VTABLE CMFCSensorManagerEvents :
  public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
  public ISensorManagerEvents
{
public:
//Constructors / Destructors
  CMFCSensorManagerEvents()
  {
  }

  DECLARE_NO_REGISTRY()

  BEGIN_COM_MAP(CMFCSensorManagerEvents)
    COM_INTERFACE_ENTRY(ISensorManagerEvents)
  END_COM_MAP()

//ISensorManagerEvents
  STDMETHOD(OnSensorEnter)(ISensor* /*pSensor*/, SensorState /*state*/)
  {
    return E_NOTIMPL;
  }
};

//The main wrapper for the ISensorManager interface
template <class SensorManagerEvents = CMFCSensorManagerEvents>
class CMFCSensorManager : public ATL::CComPtr<ISensorManager>
{
public:
//Constructors / Destructors
  CMFCSensorManager()
  {
  }

//Methods
  HRESULT Initialize()
  {
    return CoCreateInstance(CLSID_SensorManager);
  }

  HRESULT SetEventSink()
  {
    ATL::CComObject<SensorManagerEvents>* pSinkEvent = NULL;
    HRESULT hr = ATL::CComObject<SensorManagerEvents>::CreateInstance(&pSinkEvent);
    if (SUCCEEDED(hr))
      hr = p->SetEventSink(pSinkEvent);

    return hr;
  }
};

//The main wrapper for the ISensorEvents interface
class ATL_NO_VTABLE CMFCSensorEvents :
  public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
  public ISensorEvents
{
public:
//Constructors / Destructors
  CMFCSensorEvents()
  {
  }

  DECLARE_NO_REGISTRY()

  BEGIN_COM_MAP(CMFCSensorEvents)
    COM_INTERFACE_ENTRY(ISensorEvents)
  END_COM_MAP()

//ISensorEvents
  STDMETHOD(OnEvent)(ISensor* /*pSensor*/, REFGUID /*eventID*/, IPortableDeviceValues* /*pEventData*/)
  {
    return E_NOTIMPL;
  }
  STDMETHOD(OnDataUpdated)(ISensor* /*pSensor*/, ISensorDataReport* /*pNewData*/)
  {
    return E_NOTIMPL;
  }
  STDMETHOD(OnLeave)(REFSENSOR_ID /*sensorID*/)
  {
    return E_NOTIMPL;
  }
  STDMETHOD(OnStateChanged)(ISensor* /*pSensor*/, SensorState /*state*/)
  {
    return E_NOTIMPL;
  }
};

//The main wrapper for the ISensor interface
template <class SensorEvents = CMFCSensorEvents>
class CMFCSensor : public ATL::CComPtr<ISensor>
{
public:
//Constructors / Destructors
  CMFCSensor(ISensor* pSensor) : CComPtr<ISensor>(pSensor)
  {
  }
  CMFCSensor()
  {
  }

//Methods
  HRESULT SetEventSink()
  {
    ATL::CComObject<SensorEvents>* pSinkEvent = NULL;
    HRESULT hr = ATL::CComObject<SensorEvents>::CreateInstance(&pSinkEvent);
    if (SUCCEEDED(hr))
      hr = p->SetEventSink(pSinkEvent);

    return hr;
  }

  HRESULT GetFriendlyName(CString& sFriendlyName)
  {
    BSTR bstrFriendlyName = NULL;
    HRESULT hr = p->GetFriendlyName(&bstrFriendlyName);
    if (SUCCEEDED(hr))
      sFriendlyName = bstrFriendlyName;
    SysFreeString(bstrFriendlyName);
    return hr;
  }
};


//The main wrapper for ISensorDataReport
class CMFCSensorDataReport : public ATL::CComPtr<ISensorDataReport>
{
public:
//Constructors / Destructors
  CMFCSensorDataReport(ISensorDataReport* pSensorDataReport) : CComPtr<ISensorDataReport>(pSensorDataReport)
  {
  }
  CMFCSensorDataReport()
  {
  }

//Methods
  HRESULT GetValue(PROPERTYKEY propKey, double& dValue)
  {
    PROPVARIANT var;
    PropVariantInit(&var);
    HRESULT hr = p->GetSensorValue(propKey, &var);
    if (FAILED(hr))
      return hr;
    if (var.vt == VT_R8)
      dValue = var.dblVal;  
    else if (var.vt == VT_R4) //Some sensors can return a VT_R4 instead of a VT_R8 (Lets handle this issue transparently)
      dValue = var.fltVal;
    else
      hr = E_UNEXPECTED;            
    PropVariantClear(&var);

    return hr;
  }

  HRESULT GetValue(PROPERTYKEY propKey, float& dValue)
  {
    PROPVARIANT var;
    PropVariantInit(&var);
    HRESULT hr = p->GetSensorValue(propKey, &var);
    if (FAILED(hr))
      return hr;
    if (var.vt == VT_R4) //Some sensors can return a VT_R4 instead of a VT_R8 (Lets handle this issue transparently)
      dValue = var.fltVal;
    else
      hr = E_UNEXPECTED;            
    PropVariantClear(&var);

    return hr;
  }

  HRESULT GetValue(PROPERTYKEY propKey, int& nValue)
  {
    PROPVARIANT var;
    PropVariantInit(&var);
    HRESULT hr = p->GetSensorValue(propKey, &var);
    if (FAILED(hr))
      return hr;
    if (var.vt == VT_I4)
      nValue = var.lVal;
    else
      hr = E_UNEXPECTED;   
    PropVariantClear(&var);               

    return hr;
  }

  HRESULT GetValue(PROPERTYKEY propKey, bool& nValue)
  {
    PROPVARIANT var;
    PropVariantInit(&var);
    HRESULT hr = p->GetSensorValue(propKey, &var);
    if (FAILED(hr))
      return hr;
    if (var.vt == VT_BOOL)
      nValue = (var.boolVal == VARIANT_TRUE) ? true : false;
    else
      hr = E_UNEXPECTED;   
    PropVariantClear(&var);               

    return hr;
  }

  HRESULT GetValue(PROPERTYKEY propKey, ULARGE_INTEGER& nValue)
  {
    PROPVARIANT var;
    PropVariantInit(&var);
    HRESULT hr = p->GetSensorValue(propKey, &var);
    if (FAILED(hr))
      return hr;
    if (var.vt == VT_UI8)
      nValue = var.uhVal;
    else
      hr = E_UNEXPECTED;   
    PropVariantClear(&var);               

    return hr;
  }

  HRESULT GetValue(PROPERTYKEY propKey, CArray<double, double&>& dblArray)
  {
    PROPVARIANT var;
    PropVariantInit(&var);
    HRESULT hr = p->GetSensorValue(propKey, &var);
    if (FAILED(hr))
      return hr;
    if ((var.vt == (VT_VECTOR | VT_UI1)) && ((var.caub.cElems % sizeof(double)) == 0))
    {
      int nElements = var.caub.cElems/sizeof(double);
      dblArray.SetSize(nElements);
      double* pElementsToCopy = reinterpret_cast<double*>(var.caub.pElems);
      double* pArray = dblArray.GetData();
      for (int i=0; i<nElements; i++)
        pArray[i] = pElementsToCopy[i];
    }
    else
      hr = E_UNEXPECTED;   
    PropVariantClear(&var);               

    return hr;
  }

  HRESULT GetValue(PROPERTYKEY propKey, CArray<float, float&>& fltArray)
  {
    PROPVARIANT var;
    PropVariantInit(&var);
    HRESULT hr = p->GetSensorValue(propKey, &var);
    if (FAILED(hr))
      return hr;
    if ((var.vt == (VT_VECTOR | VT_UI1)) && ((var.caub.cElems % sizeof(float)) == 0))
    {
      int nElements = var.caub.cElems/sizeof(float);
      fltArray.SetSize(nElements);
      float* pElementsToCopy = reinterpret_cast<float*>(var.caub.pElems);
      float* pArray = fltArray.GetData();
      for (int i=0; i<nElements; i++)
        pArray[i] = pElementsToCopy[i];
    }
    else
      hr = E_UNEXPECTED;   
    PropVariantClear(&var);               

    return hr;
  }

  HRESULT GetValue(PROPERTYKEY propKey, CArray<ULONG, ULONG&>& ulArray)
  {
    PROPVARIANT var;
    PropVariantInit(&var);
    HRESULT hr = p->GetSensorValue(propKey, &var);
    if (FAILED(hr))
      return hr;
    if ((var.vt == (VT_VECTOR | VT_UI1)) && ((var.caub.cElems % sizeof(ULONG)) == 0))
    {
      int nElements = var.caub.cElems/sizeof(ULONG);
      ulArray.SetSize(nElements);
      ULONG* pElementsToCopy = reinterpret_cast<ULONG*>(var.caub.pElems);
      ULONG* pArray = ulArray.GetData();
      for (int i=0; i<nElements; i++)
        pArray[i] = pElementsToCopy[i];
    }
    else
      hr = E_UNEXPECTED;   
    PropVariantClear(&var);               

    return hr;
  }

  HRESULT GetValue(PROPERTYKEY propKey, CString& sValue)
  {
    PROPVARIANT var;
    PropVariantInit(&var);
    HRESULT hr = p->GetSensorValue(propKey, &var);
    if (FAILED(hr))
      return hr;
    if (var.vt == VT_LPWSTR)
      sValue = var.pwszVal;                
    else
      hr = E_UNEXPECTED;            
    PropVariantClear(&var);

    return hr;
  }
};


//A Location sensor specific wrapper for ISensorDataReport
class CMFCLocationSensorDataReport : public CMFCSensorDataReport
{
public:
//Constructors / Destructors
  CMFCLocationSensorDataReport(ISensorDataReport* pSensorDataReport) : CMFCSensorDataReport(pSensorDataReport)
  {
  }
  CMFCLocationSensorDataReport()
  {
  }

//Methods
  HRESULT GetAddress1(CString& sAddress1)
  {
    return GetValue(SENSOR_DATA_TYPE_ADDRESS1, sAddress1);
  }

  HRESULT GetAddress2(CString& sAddress2)
  {
    return GetValue(SENSOR_DATA_TYPE_ADDRESS2, sAddress2);
  }

  HRESULT GetAntennaSealevelAltitude(double& dAltitude)
  {
    return GetValue(SENSOR_DATA_TYPE_ALTITUDE_ANTENNA_SEALEVEL_METERS, dAltitude);
  }

  HRESULT GetElipsoidAltitudeError(double& dAltitude)
  {
    return GetValue(SENSOR_DATA_TYPE_ALTITUDE_ELLIPSOID_ERROR_METERS, dAltitude);
  }

  HRESULT GetElipsoidAltitude(double& dAltitude)
  {
    return GetValue(SENSOR_DATA_TYPE_ALTITUDE_ELLIPSOID_METERS, dAltitude);
  }

  HRESULT GetSealeveldAltitudeError(double& dAltitude)
  {
    return GetValue(SENSOR_DATA_TYPE_ALTITUDE_SEALEVEL_ERROR_METERS, dAltitude);
  }

  HRESULT GetSealevelAltitude(double& dAltitude)
  {
    return GetValue(SENSOR_DATA_TYPE_ALTITUDE_SEALEVEL_METERS, dAltitude);
  }

  HRESULT GetCity(CString& sCity)
  {
    return GetValue(SENSOR_DATA_TYPE_CITY, sCity);
  }

  HRESULT GetCountryOrRegion(CString& sCountryOrRegion)
  {
    return GetValue(SENSOR_DATA_TYPE_COUNTRY_REGION, sCountryOrRegion);
  }

  HRESULT GetDifferentialReferenceStationID(int& nID)
  {
    return GetValue(SENSOR_DATA_TYPE_DIFFERENTIAL_REFERENCE_STATION_ID, nID);
  }

  HRESULT GetErrorRadius(double& dErrorRadius)
  {
    return GetValue(SENSOR_DATA_TYPE_ERROR_RADIUS_METERS, dErrorRadius);
  }

  HRESULT GetFixQuality(int& nFix)
  {
    return GetValue(SENSOR_DATA_TYPE_FIX_QUALITY, nFix);
  }

  HRESULT GetDataTypeFixType(int& nFixType)
  {
    return GetValue(SENSOR_DATA_TYPE_FIX_TYPE, nFixType);
  }

  HRESULT GetGeoidalSeparation(double& dSeparation)
  {
    return GetValue(SENSOR_DATA_TYPE_GEOIDAL_SEPARATION, dSeparation);
  }

  HRESULT GetGPSOperationMode(int& nMode)
  {
    return GetValue(SENSOR_DATA_TYPE_GPS_OPERATION_MODE, nMode);
  }

  HRESULT GetGPSSelectionMode(int& nMode)
  {
    return GetValue(SENSOR_DATA_TYPE_GPS_SELECTION_MODE, nMode);
  }

  HRESULT GetGPSStatus(int& nStatus)
  {
    return GetValue(SENSOR_DATA_TYPE_GPS_STATUS, nStatus);
  }

  HRESULT GetHDOP(double& dHDOP)
  {
    return GetValue(SENSOR_DATA_TYPE_HORIZONAL_DILUTION_OF_PRECISION, dHDOP);
  }

  HRESULT GetLatitude(double& dLatitude)
  {
    return GetValue(SENSOR_DATA_TYPE_LATITUDE_DEGREES, dLatitude);
  }

  HRESULT GetLongitude(double& dLongitude)
  {
    return GetValue(SENSOR_DATA_TYPE_LONGITUDE_DEGREES, dLongitude);
  }

  HRESULT GetMagneticHeading(double& dHeading)
  {
    return GetValue(SENSOR_DATA_TYPE_MAGNETIC_HEADING_DEGREES, dHeading);
  }

  HRESULT GetMagenticVariation(double& dVariation)
  {
    return GetValue(SENSOR_DATA_TYPE_MAGNETIC_VARIATION, dVariation);
  }

  HRESULT GetNMEASentence(CString& sSentence)
  {
    return GetValue(SENSOR_DATA_TYPE_NMEA_SENTENCE, sSentence);
  }

  HRESULT GetPDOP(double& dVDOP)
  {
    return GetValue(SENSOR_DATA_TYPE_POSITION_DILUTION_OF_PRECISION, dVDOP);
  }

  HRESULT GetSatellitesInView(int& nSatellites)
  {
    return GetValue(SENSOR_DATA_TYPE_SATELLITES_IN_VIEW, nSatellites);
  }

  HRESULT GetSatellitesInViewAzimuth(CArray<double, double&>& dblArray)
  {
    return GetValue(SENSOR_DATA_TYPE_SATELLITES_IN_VIEW_AZIMUTH, dblArray);
  }

  HRESULT GetSatellitesInViewElevation(CArray<double, double&>& dblArray)
  {
    return GetValue(SENSOR_DATA_TYPE_SATELLITES_IN_VIEW_ELEVATION, dblArray);
  }

  HRESULT GetSatellitesInViewIDs(CArray<ULONG, ULONG&>& ulArray)
  {
    return GetValue(SENSOR_DATA_TYPE_SATELLITES_IN_VIEW_ID, ulArray);
  }

  HRESULT GetSatellitesUsed(int& nSatellites)
  {
    return GetValue(SENSOR_DATA_TYPE_SATELLITES_USED_COUNT, nSatellites);
  }

  HRESULT GetSatellitesUsedPRNS(CArray<ULONG, ULONG&>& ulArray)
  {
    return GetValue(SENSOR_DATA_TYPE_SATELLITES_USED_PRNS, ulArray);
  }

  HRESULT GetSatellitesInViewSTNRatio(CArray<double, double&>& dblArray)
  {
    return GetValue(SENSOR_DATA_TYPE_SATELLITES_IN_VIEW_STN_RATIO, dblArray);
  }

  HRESULT GetSpeed(double& dSpeed)
  {
    return GetValue(SENSOR_DATA_TYPE_SPEED_KNOTS, dSpeed);
  }

  HRESULT GetStateProvince(CString& sStateProvince)
  {
    return GetValue(SENSOR_DATA_TYPE_STATE_PROVINCE, sStateProvince);
  }

  HRESULT GetTrueHeading(double& dHeading)
  {
    return GetValue(SENSOR_DATA_TYPE_TRUE_HEADING_DEGREES, dHeading);
  }

  HRESULT GetVDOP(double& dVDOP)
  {
    return GetValue(SENSOR_DATA_TYPE_VERTICAL_DILUTION_OF_PRECISION, dVDOP);
  }
};


//An Electrical sensor specific wrapper for ISensorDataReport
class CMFCElectricalSensorDataReport : public CMFCSensorDataReport
{
public:
//Constructors / Destructors
  CMFCElectricalSensorDataReport(ISensorDataReport* pSensorDataReport) : CMFCSensorDataReport(pSensorDataReport)
  {
  }
  CMFCElectricalSensorDataReport()
  {
  }

//Methods
  HRESULT GetCapacitance(double& dCapacitance)
  {
    return GetValue(SENSOR_DATA_TYPE_CAPACITANCE_FARAD, dCapacitance);
  }

  HRESULT GetCurrent(double& dCurrent)
  {
    return GetValue(SENSOR_DATA_TYPE_CURRENT_AMPS, dCurrent);
  }

  HRESULT GetPower(double& dPower)
  {
    return GetValue(SENSOR_DATA_TYPE_ELECTRICAL_POWER_WATTS, dPower);
  }

  HRESULT GetInductance(double& dInductance)
  {
    return GetValue(SENSOR_DATA_TYPE_INDUCTANCE_HENRY, dInductance);
  }

  HRESULT GetResistance(double& dResistance)
  {
    return GetValue(SENSOR_DATA_TYPE_RESISTANCE_OHMS, dResistance);
  }

  HRESULT GetVoltage(double& dVoltage)
  {
    return GetValue(SENSOR_DATA_TYPE_VOLTAGE_VOLTS, dVoltage);
  }
};


//A Environmental sensor specific wrapper for ISensorDataReport
class CMFCEnvironmentalSensorDataReport : public CMFCSensorDataReport
{
public:
//Constructors / Destructors
  CMFCEnvironmentalSensorDataReport(ISensorDataReport* pSensorDataReport) : CMFCSensorDataReport(pSensorDataReport)
  {
  }
  CMFCEnvironmentalSensorDataReport()
  {
  }

//Methods
  HRESULT GetAtmosphericPressure(float& fPressure)
  {
    return GetValue(SENSOR_DATA_TYPE_ATMOSPHERIC_PRESSURE_BAR, fPressure);
  }

  HRESULT GetTemperature(float& fTemperature)
  {
    return GetValue(SENSOR_DATA_TYPE_TEMPERATURE_CELSIUS, fTemperature);
  }

  HRESULT GetRelativeHumidity(float& fRelativeHumidity)
  {
    return GetValue(SENSOR_DATA_TYPE_RELATIVE_HUMIDITY_PERCENT, fRelativeHumidity);
  }

  HRESULT GetWindDirection(float& fDirection)
  {
    return GetValue(SENSOR_DATA_TYPE_WIND_DIRECTION_DEGREES_ANTICLOCKWISE, fDirection);
  }

  HRESULT GetWindSpeed(float& fSpeed)
  {
    return GetValue(SENSOR_DATA_TYPE_WIND_SPEED_METERS_PER_SECOND, fSpeed);
  }
};

//A Light sensor specific wrapper for ISensorDataReport
class CMFCLightSensorDataReport : public CMFCSensorDataReport
{
public:
//Constructors / Destructors
  CMFCLightSensorDataReport(ISensorDataReport* pSensorDataReport) : CMFCSensorDataReport(pSensorDataReport)
  {
  }
  CMFCLightSensorDataReport()
  {
  }

//Methods
  HRESULT GetChromacity(CArray<float, float&>& fltArray)
  {
    return GetValue(SENSOR_DATA_TYPE_LIGHT_CHROMACITY, fltArray);
  }

  HRESULT GetLevel(float& fLevel)
  {
    return GetValue(SENSOR_DATA_TYPE_LIGHT_LEVEL_LUX, fLevel);
  }

  HRESULT GetTemperature(float& fTemperature)
  {
    return GetValue(SENSOR_DATA_TYPE_LIGHT_TEMPERATURE_KELVIN, fTemperature);
  }
};


//A Mechanical sensor specific wrapper for ISensorDataReport
class CMFCMechanicalSensorDataReport : public CMFCSensorDataReport
{
public:
//Constructors / Destructors
  CMFCMechanicalSensorDataReport(ISensorDataReport* pSensorDataReport) : CMFCSensorDataReport(pSensorDataReport)
  {
  }
  CMFCMechanicalSensorDataReport()
  {
  }

//Methods
  HRESULT GetAbsolutePressure(double& dAbsolutePressure)
  {
    return GetValue(SENSOR_DATA_TYPE_ABSOLUTE_PRESSURE_PASCAL, dAbsolutePressure);
  }

  HRESULT GetSwitchState(bool& bState)
  {
    return GetValue(SENSOR_DATA_TYPE_BOOLEAN_SWITCH_STATE, bState);
  }

  HRESULT GetForce(double& dForce)
  {
    return GetValue(SENSOR_DATA_TYPE_FORCE_NEWTONS, dForce);
  }

  HRESULT GetGaugePressure(double& dGaugePressure)
  {
    return GetValue(SENSOR_DATA_TYPE_GAUGE_PRESSURE_PASCAL, dGaugePressure);
  }

  HRESULT GetMutiValueSwitchState(double& dSwitchState)
  {
    return GetValue(SENSOR_DATA_TYPE_MULTIVALUE_SWITCH_STATE, dSwitchState);
  }

  HRESULT GetStrain(double& dStrain)
  {
    return GetValue(SENSOR_DATA_TYPE_STRAIN, dStrain);
  }

  HRESULT GetWeight(double& dWeight)
  {
    return GetValue(SENSOR_DATA_TYPE_WEIGHT_KILOGRAMS, dWeight);
  }
};


//A Motion sensor specific wrapper for ISensorDataReport
class CMFCMotionSensorDataReport : public CMFCSensorDataReport
{
public:
//Constructors / Destructors
  CMFCMotionSensorDataReport(ISensorDataReport* pSensorDataReport) : CMFCSensorDataReport(pSensorDataReport)
  {
  }
  CMFCMotionSensorDataReport()
  {
  }

//Methods
  HRESULT GetAccelerationX(double& dAccelerationX)
  {
    return GetValue(SENSOR_DATA_TYPE_ACCELERATION_X_G, dAccelerationX);
  }

  HRESULT GetAccelerationY(double& dAccelerationY)
  {
    return GetValue(SENSOR_DATA_TYPE_ACCELERATION_Y_G, dAccelerationY);
  }

  HRESULT GetAccelerationZ(double& dAccelerationZ)
  {
    return GetValue(SENSOR_DATA_TYPE_ACCELERATION_Z_G, dAccelerationZ);
  }

  HRESULT GetGyrometricAccelerationX(double& dGyrometricAccelerationX)
  {
    return GetValue(SENSOR_DATA_TYPE_ANGULAR_ACCELERATION_X_DEGREES_PER_SECOND_SQUARED, dGyrometricAccelerationX);
  }

  HRESULT GetGyrometricAccelerationY(double& dGyrometricAccelerationY)
  {
    return GetValue(SENSOR_DATA_TYPE_ANGULAR_ACCELERATION_Y_DEGREES_PER_SECOND_SQUARED, dGyrometricAccelerationY);
  }

  HRESULT GetGyrometricAccelerationZ(double& dGyrometricAccelerationZ)
  {
    return GetValue(SENSOR_DATA_TYPE_ANGULAR_ACCELERATION_Z_DEGREES_PER_SECOND_SQUARED, dGyrometricAccelerationZ);
  }

  HRESULT GetState(bool& bState)
  {
    return GetValue(SENSOR_DATA_TYPE_MOTION_STATE, bState);
  }

  HRESULT GetSpeed(double& dSpeed)
  {
    return GetValue(SENSOR_DATA_TYPE_SPEED_METERS_PER_SECOND, dSpeed);
  }
};


//A Orientation sensor specific wrapper for ISensorDataReport
class CMFCOrientationSensorDataReport : public CMFCSensorDataReport
{
public:
//Constructors / Destructors
  CMFCOrientationSensorDataReport(ISensorDataReport* pSensorDataReport) : CMFCSensorDataReport(pSensorDataReport)
  {
  }
  CMFCOrientationSensorDataReport()
  {
  }

//Methods
  HRESULT GetTiltX(float& fTiltX)
  {
    return GetValue(SENSOR_DATA_TYPE_TILT_X_DEGREES, fTiltX);
  }

  HRESULT GetTiltY(float& fTiltY)
  {
    return GetValue(SENSOR_DATA_TYPE_TILT_Y_DEGREES, fTiltY);
  }

  HRESULT GetTiltZ(float& fTiltZ)
  {
    return GetValue(SENSOR_DATA_TYPE_TILT_Z_DEGREES, fTiltZ);
  }

  HRESULT GetDistanceX(float& fDistanceX)
  {
    return GetValue(SENSOR_DATA_TYPE_DISTANCE_X_METERS, fDistanceX);
  }

  HRESULT GetDistanceY(float& fDistanceY)
  {
    return GetValue(SENSOR_DATA_TYPE_DISTANCE_Y_METERS, fDistanceY);
  }

  HRESULT GetDistanceZ(float& fDistanceZ)
  {
    return GetValue(SENSOR_DATA_TYPE_DISTANCE_Z_METERS, fDistanceZ);
  }

  HRESULT GetMagneticHeadingX(float& fMagenticHeadingX)
  {
    return GetValue(SENSOR_DATA_TYPE_MAGNETIC_HEADING_X_DEGREES, fMagenticHeadingX);
  }

  HRESULT GetMagneticHeadingY(float& fMagenticHeadingY)
  {
    return GetValue(SENSOR_DATA_TYPE_MAGNETIC_HEADING_Y_DEGREES, fMagenticHeadingY);
  }

  HRESULT GetMagneticHeadingZ(float& fMagenticHeadingZ)
  {
    return GetValue(SENSOR_DATA_TYPE_MAGNETIC_HEADING_Z_DEGREES, fMagenticHeadingZ);
  }
};


//A Scanner sensor specific wrapper for ISensorDataReport
class CMFCScannerSensorDataReport : public CMFCSensorDataReport
{
public:
//Constructors / Destructors
  CMFCScannerSensorDataReport(ISensorDataReport* pSensorDataReport) : CMFCSensorDataReport(pSensorDataReport)
  {
  }
  CMFCScannerSensorDataReport()
  {
  }

//Methods
  HRESULT GetRFIDTag(ULARGE_INTEGER& uhVal)
  {
    return GetValue(SENSOR_DATA_TYPE_RFID_TAG_40_BIT, uhVal);
  }
};


#endif //__MFCSENSOR_H__