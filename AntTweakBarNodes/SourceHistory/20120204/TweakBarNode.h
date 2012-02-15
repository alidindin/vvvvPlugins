// 
// TweakBarNode.h
// 
// vvvv��AntTweakBar���g���B
// 
// updated: 2012/02/04
// written: Minoru Ito

#pragma once

//----------------------�K�v�Ȗ��O��Ԃ�錾
using namespace System::Drawing;	//RGBAColor���g���Ƃ��ꂪ�K�v�ɂȂ�B�Ȃ��H
using namespace System::Collections::Generic;

using namespace SlimDX;
using namespace SlimDX::Direct3D9;
using namespace VVVV::PluginInterfaces::V2::EX9;
using namespace VVVV::Utils::SlimDX;

//TweakBar��OutputPin�Ɏg����悤�ɂ���
#include "TweakBarDataType.h"
using namespace VVVV::DataTypes;


//�^���Ƃ̍ő吔�i�b��I�Ɏg�p�j
#define MAX_COUNT 10


namespace VVVV
{
	namespace Nodes
	{

		//http://akatukisiden.wordpress.com/2011/12/23/cpp-ccli-wrapping-object/
		//�ϐ���ێ�����A���}�l�[�W�h�N���X
		//�����new�������̂̓K�x�[�W�R���N�^�ɊĎ�����Ȃ��̂Ŏ蓮�ŊǗ�����B
		public class UnmanagedClass
		{
		public:
			bool toggleArray[MAX_COUNT];
			int integerArray[MAX_COUNT];
			double doubleArray[MAX_COUNT];
			//char* stringArray[MAX_COUNT];
			float colorArray[MAX_COUNT][4];			//4��float������2�����z��iRGBA�j
			double directionArray[MAX_COUNT][3];	//3��double������2�����z��iXYZ�j
			double quaternionArray[MAX_COUNT][4];	//4��double������2�����z��iXYZW�j
		};

		//enum�F�e�L�X�g�J���[
		enum class TextEnum
		{
			Dark,
			Light
		};

		//enum�F�f�[�^�^
		enum class TypeEnum
		{
			Toggle,
			Integer,
			Double,
			String,
			Color,
			Direction,
			Quaternion,
			//Separator
		};

		
		//PluginInfo�̐ݒ�i���O�A�J�e�S���A�w���v�A�^�O�Ȃǁj
		[PluginInfo(Name = "TweakBar", Category = "AntTweakBar", Help = "", Tags = "", Author = "mino")]
		//�N���X�錾
		public ref class TweakBarNode : DXLayerOutPluginBase<DeviceData^>, IPluginEvaluate
		{

		public:

			//�R���X�g���N�^
			[ImportingConstructor()]
			TweakBarNode(IPluginHost2^ host);
			
			//�f�X�g���N�^
			~TweakBarNode();

			
			//----------------------�K�{��public���\�b�h
			//IPluginEvaluate
			virtual void Evaluate(int SpreadMax);
			//DXLayerOutPluginBase
			virtual DeviceData^ CreateDeviceData(Device^ device) override;
			virtual void UpdateDeviceData(DeviceData^ deviceData) override;
			virtual void DestroyDeviceData(DeviceData^ deviceData, bool OnlyUnManaged) override;
			virtual void Render(Device^ device, DeviceData^ deviceData) override;

		//internal:

		protected:

			//�t�@�C�i���C�U
			!TweakBarNode();
			
		private:

			void InitTweakBar(Device^ device);
			void CreateKeyEvent(int keyCodeWin32, int modifiers);

			

			/*
			//�}�l�[�W�ƃA���}�l�[�W��callBack�Ȃǂ�╡�G�Ȃ̂łƂ肠�����ۗ�
			//----------------------TweakBar��callBack function
			void TW_CALL ButtonCallback(void *clientData);
			*/

			
			//---------------------------------���̓s��
			// 
			// IDiffSpread��ISpread���p�����Ă���B�O��̃t���[������ω��������ǂ����𓾂���


			//---------�C�x���g�����֘A

			//�}�E�X���{�^��
			[Input("Mouse Button", IsSingle = true)]
			IDiffSpread<bool>^ FMouseButton;
			
			//�}�E�X���W
			[Input("Mouse XY", DefaultValue = 0.0)]
			IDiffSpread<double>^ FMouseXY;
			
			//�L�[�R�[�h�iWin32 API�̉��z�L�[�R�[�h�j
			[Input("KeyCode")]
			IDiffSpread<int>^ FKeyCode;
			
			//Renderer�T�C�Y
			[Input("Window Size", DefaultValue = 400)]
			IDiffSpread<int>^ FWindowSize;
			

			//---------�O���[�o���ݒ�֘A

			//�^�C�g��
			[Input("Title", IsSingle = true, DefaultString = "TweakBar")]
			IDiffSpread<String^>^ FTitle;
			
			//�w�i�F
			//TweakBar�̏����ݒ�F��(R:17, G:109, B:143, A:64)
			[Input("Color", DefaultColor = gcnew array<double>{(double)17/255, (double)109/255, (double)143/255, (double)64/255})]
			IDiffSpread<RGBAColor>^ FColor;
			
			//�e�L�X�g�\��
			[Input("Text", DefaultEnumEntry = "Light", IsSingle = true)]
			IDiffSpread<TextEnum>^ FText;


			//---------�p�����[�^�ݒ�֘A

			//�ǉ�����ϐ��̃f�[�^�^
			[Input("Add Type", DefaultEnumEntry = "Double")]
			IDiffSpread<TypeEnum>^ FType;

			//�ǉ�����ϐ��̖��O
			[Input("Add Name")]
			IDiffSpread<String^>^ FAddName;

			//�ǉ�����ϐ��̃I�v�V����
			[Input("Add Parameters")]
			IDiffSpread<String^>^ FOptionParameters;

			//�ϐ��̒ǉ�
			[Input("Add", IsSingle = true, IsBang = true)]
			ISpread<bool>^ FAdd;
			
			//�폜����ϐ��̖��O
			[Input("Remove Name")]
			IDiffSpread<String^>^ FRemoveName;

			//�ϐ��̍폜
			[Input("Remove", IsSingle = true, IsBang = true)]
			ISpread<bool>^ FRemove;
			
			//�{�^���ǉ��e�X�g
			//�}�l�[�W�ƃA���}�l�[�W��callBack�Ȃǂ�╡�G�Ȃ̂łƂ肠�����ۗ�
			//[Input("Add Button", IsSingle = true, IsBang = true)]
			//ISpread<bool>^ FAddButton;
			
			//���ǉ��e�X�g�FAdd Variable�Ɠ�������H
			[Input("Add Separator", IsSingle = true, IsBang = true)]
			ISpread<bool>^ FAddSeparator;


			//---------���̂ق�

			[Input("Enabled", DefaultValue = 1.0, IsSingle = true)]
			ISpread<bool>^ FEnabled;

			//[Input("Transform In")]
			//IDiffSpread<Matrix>^ FTransformIn;

			//[Input("Vertex Positions")]
			//IDiffSpread<Vector3>^ FVertexIn;

			//[Input("Texture Filename", StringType = StringType::Filename)]
			//IDiffSpread<String^>^ FTexFileName;


			//---------------------------------�o�̓s��

			[Output("TweakBar", IsSingle = true)]
			ISpread<TweakBarDataType^>^ FTweakBarOutput;
			
			//�o�^�ς݂̕ϐ���
			[Output("Toggle Name")]
			ISpread<String^>^ FToggleName;

			[Output("Integer Name")]
			ISpread<String^>^ FIntegerName;

			[Output("Double Name")]
			ISpread<String^>^ FDoubleName;

			[Output("String Name")]
			ISpread<String^>^ FStringName;

			[Output("Color Name")]
			ISpread<String^>^ FColorName;

			[Output("Direction Name")]
			ISpread<String^>^ FDirectionName;

			[Output("Quaternion Name")]
			ISpread<String^>^ FQuaternionName;

			
			//----------------------ILogger
			[Import()]
			ILogger^ FLogger;


			//----------------------���̑��̃t�B�[���h
			
			//TweakBar
			TwBar* bar;

			//TweakBar��Pin�ň������߂̃N���X
			TweakBarDataType^ FTweakBarDataType;

			//�ϐ����A���}�l�[�W�h�ɊǗ�����N���X
			UnmanagedClass* unmanagedClass;

			//PluginHost
			IPluginHost2^ FHost;

			//slice count
			//int FSpreadCount;

			//check connect
			bool isConnected;

			/*
			//�ϐ���ێ�����array
			array<bool>^ FArrayToggle;
			array<int>^ FArrayInteger;
			array<double>^ FArrayDouble;
			array<char*>^ FArrayString;
			array<float, 2>^ FArrayColor;
			array<double, 2>^ FArrayDirection;
			array<double, 2>^ FArrayQuaternion;

			//�ϐ����J�E���g����
			int FCounterBool;
			int FCounterInt;
			int FCounterDouble;
			int FCounterString;
			int FCounterColor;
			int FCounterDirection;
			int FCounterQuaternion;
			*/

			//�ϐ����Ɣz���index��Ή��t���邽�߂�Dictionary
			Dictionary<String^, int>^ FDictToggle;
			Dictionary<String^, int>^ FDictInteger;
			Dictionary<String^, int>^ FDictDouble;
			Dictionary<String^, int>^ FDictString;
			Dictionary<String^, int>^ FDictColor;
			Dictionary<String^, int>^ FDictDirection;
			Dictionary<String^, int>^ FDictQuaternion;
		};
	}
}