// 
// TweakBarVariables.h
// 
// holding variables for AntTweakBar
// 
// updated: 2012/02/05
// written: Minoru Ito

#pragma once

using namespace System::Collections::Generic;	// for Dictionary


// max count of each variables (temporary)
#define MAX_COUNT 10

namespace VVVV
{
	namespace DataTypes
	{
		//http://akatukisiden.wordpress.com/2011/12/23/cpp-ccli-wrapping-object/
		//Unmanaged class for variables.
		//you must check manually because instances are NOT checked by GC.
		public class UnmanagedClass
		{
		public:

			bool toggleArray[MAX_COUNT];
			int integerArray[MAX_COUNT];
			double doubleArray[MAX_COUNT];
			//char* stringArray[MAX_COUNT];
			float colorArray[MAX_COUNT][4];			//4 float for RGBA
			double directionArray[MAX_COUNT][3];	//3 double for XYZ
			double quaternionArray[MAX_COUNT][4];	//4 double for XYZW
		};


		//managed class
		public ref class TweakBarVariables
		{
		public:

			//Constructor
			TweakBarVariables();
			
			//Destructor
			~TweakBarVariables();

			//add variable
			void AddVariable(String^ type, String^ name, int index);

			//remove variable
			void RemoveVariable(String^ name);

			//find variable
			bool FindVariable(String^ name);

			//get index of variable
			int GetIndex(String^ name);

			//get empty index
			int GetEmptyIndex(String^ type);

			//get pointer of variable
			//本当はindexではなくnameで取得できるようにしたい
			void* GetPointer(String^ type, int index);

			//get type of variable
			String^ GetType(String^ name);

			//get variable
			bool GetToggle(String^ name);
			int GetInteger(String^ name);
			double GetDouble(String^ name);
			//void GetString(String^ name);
			RGBAColor^ GetColor(String^ name);
			Vector3D^ GetDirection(String^ name);
			Vector4D^ GetQuaternion(String^ name);

		protected:

			//Finalizer
			!TweakBarVariables();

		private:
			
			//Dictionary for variable name and index of unmanaged array
			Dictionary<String^, int>^ FDictToggle;
			Dictionary<String^, int>^ FDictInteger;
			Dictionary<String^, int>^ FDictDouble;
			Dictionary<String^, int>^ FDictString;
			Dictionary<String^, int>^ FDictColor;
			Dictionary<String^, int>^ FDictDirection;
			Dictionary<String^, int>^ FDictQuaternion;

			//変数をアンマネージドに管理するクラス
			UnmanagedClass* unmanagedClass;

		};
	}
}