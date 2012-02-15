#include "stdafx.h"

#include "TweakBarNode.h"

using namespace VVVV::Nodes;



// import host and hand it to base constructor
// the two booleans set whether to create a render state and/or sampler state pin
TweakBarNode::TweakBarNode(IPluginHost2^ host) : DXLayerOutPluginBase(host, false, false)
{
	//set IPluginHost2 to filed
	//you can use like FHost->GetID() with it
	FHost = host;

	//create TweakBarVariables instance
	FVariables = gcnew TweakBarVariables();

	//set order of FLayerOut
	//this->FLayerOut->Order = 0;
}

//�f�X�g���N�^�F���\�[�X�A���ɃA���}�l�[�W�x���\�[�X��ϋɓI�ɉ�����邽�߂̃��\�b�h
//.NET Framework��IDisposable.Dospose()�ɑ�������
//�}�l�[�W�h���\�[�X��������ăt�@�C�i���C�U���Ăяo���̂��ǂ�
TweakBarNode::~TweakBarNode()
{
	//FLogger->Log(LogType::Debug, "I am from Destructor");

	//�����Ń}�l�[�W�h���\�[�X���f�X�g���N�g

	//�t�@�C�i���C�U�̎��s
	this->!TweakBarNode();
}

//�t�@�C�i���C�U�F���\�[�X�̉�����Y��ɂ���Q���ŏ����ɗ}���邽�߂̃��\�b�h
//.NET Framework��Object.Finalize()�ɑ�������
//�A���}�l�[�W�h���\�[�X���������
TweakBarNode::!TweakBarNode()
{
	//FLogger->Log(LogType::Debug, "I am from Finalizer");

	//�����ŃA���}�l�[�W�h���\�[�X���f�X�g���N�g
	
}



//called when data for any output pin is requested
void TweakBarNode::Evaluate(int SpreadMax)
{
	//FLogger->Log(LogType::Debug, "I am from Evaluate");

	//�\���E��\���̐؂�ւ��iTweakBar���̂̕`��Ƃ͕����Ă���j
	if(FEnabled->IsChanged)
	{
		int visible;

		((bool)FEnabled[0])? visible = 1 : visible = 0;

		TwSetParam(bar, NULL, "visible", TW_PARAM_INT32, 1, &visible);
	}

	if(FLayerOut->IsConnected && FEnabled[0])
	{
		//�E�B���h�E�T�C�Y�̕ω��𒲂ׂ�
		if(FWindowSize->IsChanged)
		{
			//call TweakBar event
			TwWindowSize((int)FWindowSize[0], (int)FWindowSize[1]);
		}

		//�}�E�X���W�̕ω��𒲂ׂ�
		if(FMouseXY->IsChanged)
		{
			//TweakBar�ɍ��킹�ă}�E�X���W��ݒ�
			int mouseX = (int)(((double)FMouseXY[0] + 1) * ((int)FWindowSize[0] * 0.5));
			int mouseY = (int)((1 - (double)FMouseXY[1]) * ((int)FWindowSize[1] * 0.5));

			//call TweakBar event
			TwMouseMotion(mouseX, mouseY);
		}

		//�}�E�X�{�^���̕ω��𒲂ׂ�
		if(FMouseButton->IsChanged)
		{
			int result;

			//�ω���̒l��true�F�����ꂽ
			if((bool)FMouseButton[0])
			{
				//call TweakBar event
				result = TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT);
			}
			//false�F���ꂽ
			else
			{
				//call TweakBar event
				result = TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT);
			}

			FLogger->Log(LogType::Debug, FHost->GetID() + ":" + result.ToString());
		}

		//�L�[�{�[�h�̕ω��𒲂ׂ�
		// �����̃L�[�������ɉ����ꂽ�ꍇ�i�R���g���[���L�[�Ȃǂ��܂ށj�́A
		// Spread�œn�����̂ł���ɉ������������K�v
		if(FKeyCode->IsChanged)
		{
			int code, modifier;
			
			code = (int)FKeyCode[0];

			if(code != 0)
			{
				switch(code)
				{
					case 16:	//SHIFT
						modifier = TW_KMOD_SHIFT;
						code = (int)FKeyCode[1];
						break;
					case 17:	//CONTROL
						modifier = TW_KMOD_CTRL;
						code = (int)FKeyCode[1];
						break;
					case 18:	//ALT
						modifier = TW_KMOD_ALT;
						code = (int)FKeyCode[1];
						break;
					default:
						modifier = TW_KMOD_NONE;
						break;
				}
				//�L�[�C�x���g���쐬
				if(code != 0) this->CreateKeyEvent(code, modifier);
			}
		}

		//�^�C�g���̕ω��𒲂ׂ�
		if(FTitle->IsChanged)
		{
			//System::String^����char*�֕ϊ�
			char* pStr = (char *)Marshal::StringToHGlobalAnsi((String^)FTitle[0]).ToPointer();

			//call TwSetParam
			TwSetParam(bar, NULL, "label", TW_PARAM_CSTRING, 1, pStr);

			//�������̉��
			Marshal::FreeHGlobal(IntPtr(pStr));
		}

		//�F�̕ω��𒲂ׂ�
		if(FColor->IsChanged)
		{
			RGBAColor color = (RGBAColor)FColor[0];
			int intColor[3], intAlpha;

			//mapping 0 - 1 to 0 - 255
			intColor[0] = (int)(color.R * 255);
			intColor[1] = (int)(color.G * 255);
			intColor[2] = (int)(color.B * 255);
			intAlpha = (int)(color.A * 255);

			//call TwSetParam
			TwSetParam(bar, NULL, "color", TW_PARAM_INT32, 3, intColor);
			TwSetParam(bar, NULL, "alpha", TW_PARAM_INT32, 1, &intAlpha);
		}


		/*
		//�}�l�[�W�ƃA���}�l�[�W��callBack�Ȃǂ�╡�G�Ȃ̂łƂ肠�����ۗ�
		//�{�^����ǉ�����
		if(FAddButton[0])
		{
			//call TwAddButton
			TwAddButton(bar, "Button", ButtonCallback, NULL, NULL);
		}
		*/

		/*
		//����ǉ�����F�ϐ��̒ǉ��ɓ���
		if(FAddSeparator[0])
		{
			//call TwAddSeparator
			TwAddSeparator(bar, NULL, NULL);
		}
		*/

		//�ϐ��̒ǉ�
		if(FAdd[0])
		{
			for (int i = 0; i < FAddName->SliceCount; i++)
			{
				//�ϐ���ǉ�����
				this->AddVariable(Enum::GetName(TypeEnum::typeid, FType[i]), (String^)FAddName[i], (String^)FAddParameters[i]);
			}
		}

		//�ϐ��̍폜
		if(FRemove[0])
		{
			for (int i = 0; i < FRemoveName->SliceCount; i++)
			{
				//�ϐ����폜����
				this->RemoveVariable((String^)FRemoveName[i]);
			}
		}

		//�e�L�X�g�\���̕ύX
		if(FText->IsChanged)
		{
			//System::String^����char*�֕ϊ�
			String^ str = Enum::GetName(TextEnum::typeid, FText[0]);
			char* pStr = (char *)Marshal::StringToHGlobalAnsi(str).ToPointer();

			//call TwSetParam
			TwSetParam(bar, NULL, "text", TW_PARAM_CSTRING, 1, pStr);

			//�������̉��
			Marshal::FreeHGlobal(IntPtr(pStr));
		}
		
		/*
		//TweakBar�̖��O��vvvv�Ŋm�F���邽�߂̂���
		FStringOutput[0] = gcnew String(TwGetBarName(bar));
		
		//TweakBar�̐F��vvvv�Ŋm�F���邽�߂̂���
		int col[3], alpha;
		TwGetParam(bar, NULL, "color", TW_PARAM_INT32, 3, col);
		TwGetParam(bar, NULL, "alpha", TW_PARAM_INT32, 1, &alpha);
		FLogger->Log(LogType::Debug, "r:" + col[0].ToString());
		FLogger->Log(LogType::Debug, "a:" + alpha.ToString());
		*/

	}//end of if(_isConnected)
}

//this method gets called, when Reinitialize() was called in evaluate,
//or a graphics device asks for its data
DeviceData^ TweakBarNode::CreateDeviceData(Device^ device)
{
	//FLogger->Log(LogType::Debug, "I am from CreateDeviceData");

	//TweakBar�̏�����
	this->InitTweakBar(device);

	//TweakBarVariables���Z�b�g
	FVariablesOutput[0] = FVariables;

	//StateBlock���쐬�iRender���\�b�h���ōs���ƕ`�悪�d���Ȃ�̂ł����Łj
	//�Q�l�Fhttp://msdn.microsoft.com/ja-jp/library/cc324052.aspx
	block = gcnew StateBlock(device, StateBlockType::All);

	//return a new device data
	return gcnew DeviceData();
}

//this method gets called, when Update() was called in evaluate,
//or a graphics device asks for its data, here you fill the resources with the actual data
void TweakBarNode::UpdateDeviceData(DeviceData^ deviceData)
{
	//FLogger->Log(LogType::Debug, "I am from UpdateDeviceData");
}

//this is called by vvvv to delete the resources of a specific device data
void TweakBarNode::DestroyDeviceData(DeviceData^ deviceData, bool OnlyUnManaged)
{
	//FLogger->Log(LogType::Debug, "I am from DestroyDeviceData");

	// Delete TweakBar
	if(!TwDeleteBar(bar))
	{
		FMessage[0] = gcnew String(TwGetLastError());
	}
	else
	{
		FMessage[0] = "TweakBar Deleted";

		//�J�E���g�_�E��
		GBarCount--;
	}
	
	//bar count = 1 means only help bar left
	//if(TwGetBarCount() == 1)
	if(GBarCount == 0)
	{
		// Terminate AntTweakBar
		if(!TwTerminate())
		{
			FMessage[0] = gcnew String(TwGetLastError());
		}
		else
		{
			FMessage[0] = "TweakBar Terminated";
		}

		//Reset Counter
		GRenderCounter = 0;
	}

	//Clear Dictionaries
	FVariables->ClearAllDictionary();

	//Dispose StateBlock
	delete block;
}

//render into the vvvv renderer
void TweakBarNode::Render(Device^ device, DeviceData^ deviceData)
{
	//FLogger->Log(LogType::Debug, "I am from Render");

	//NOTE:
	//  AntTweakBar lib save current state(like ZEnable) before Drawing,
	//  and load saved state after Drawing in TwDraw() method.
	//  In vvvv (or SlimDX?) it seems NOT working.
	//  So you need save & load manually with SlimDX lib.
	//  It can do with Capture() & Apply() methods od StateBlock class.
	//  http://slimdx.org/docs/#T_SlimDX_Direct3D9_StateBlock
	

	//	before = device->GetRenderState(SlimDX::Direct3D9::RenderState::ZEnable);
	//	FLogger->Log(LogType::Debug, "before: " + before);
	//
	//	//getting ZEnable status by string
	//	String^ before = Enum::GetName(SlimDX::Direct3D9::ZBufferType::typeid, device->GetRenderState(SlimDX::Direct3D9::RenderState::ZEnable));

	
	//TwDraw() should call only once at one loop.
	//GRenderCounter needs when multiple tweakbar exists
	if(GRenderCounter == 0)
	{
		//�f�o�C�X�̌��݂̃X�e�[�g���擾����
		block->Capture();

		// Draw tweak bars
		TwDraw();

		//�X�e�[�g�u���b�N�����݂̃f�o�C�X�X�e�[�g�ɓK�p����
		block->Apply();
	}

	//Reset counter if equals tweakbar count
	if(++GRenderCounter == GBarCount) GRenderCounter = 0;


	//  device->SetRenderState(SlimDX::Direct3D9::RenderState::ZEnable, SlimDX::Direct3D9::ZBufferType::UseZBuffer);
	//
	//	after = device->GetRenderState(SlimDX::Direct3D9::RenderState::ZEnable);
	//	FLogger->Log(LogType::Debug, "after:" + after.ToString());
	//	if(before != after) FLogger->Log(LogType::Debug, "change ZEnable");
}

//TweakBar�̏�����
void TweakBarNode::InitTweakBar(Device^ device)
{
	//---------------------------------------------------------AntTweakBar�̏�����
    //TweakBar�̐���0�̏ꍇ�̂ݍs��
	if(TwGetBarCount() == 0)
	{
		// (note that the Direct3D device pointer must be passed to TwInit)
		if( !TwInit(TW_DIRECT3D9, (void *)device->ComPointer))
		{
			String^ str = gcnew String(TwGetLastError());
			
			FMessage[0] = "TweakBar Init failed: " + str;

			delete str;
		}
		else
		{
			FMessage[0] = "TweakBar Init successed";
		}

		//---------------------------------------------------------��ʃT�C�Y���Z�b�g
		TwWindowSize((int)FWindowSize[0], (int)FWindowSize[1]);
		/*
		if(!TwWindowSize(FHoldWidth, FHoldHeight))
		{
			String^ str = gcnew String(TwGetLastError());
			FLogger->Log(LogType::Debug, str);
			delete str;
		}
		else
		{
			FLogger->Log(LogType::Debug, "TwWindowSize OK");
		}
		*/
	}
	

	//---------------------------------------------------------TweakBar�̍쐬
	//������TweakBar���쐬�������ɖ��O�����Ԃ�Ȃ��悤�ɂ���K�v������
	
	//System::String^��TweakBar�̖��O��ݒ肵�Achar*�֕ϊ�
	char* name = (char *)Marshal::StringToHGlobalAnsi( "TweakBar" + FHost->GetID().ToString() ).ToPointer();
	
	//TweakBar�̍쐬
	bar = TwNewBar(name);

	//�������̉��
	Marshal::FreeHGlobal(IntPtr(name));

	//�J�E���g�A�b�v
	GBarCount++;

	//---------------------------------------------------------�^�C�g�����Z�b�g
	//System::String^����char*�֕ϊ�
	char* label = (char *)Marshal::StringToHGlobalAnsi((String^)FTitle[0]).ToPointer();

	//call TwSetParam
	TwSetParam(bar, NULL, "label", TW_PARAM_CSTRING, 1, label);

	//�������̉��
	Marshal::FreeHGlobal(IntPtr(label));
	//TwDefine(" TweakBar label='Settings' ");


	//---------------------------------------------------------�F���Z�b�g
	RGBAColor color = (RGBAColor)FColor[0];
	int intColor[3], intAlpha;

	//mapping 0 - 1 to 0 - 255
	intColor[0] = (int)(color.R * 255);
	intColor[1] = (int)(color.G * 255);
	intColor[2] = (int)(color.B * 255);
	intAlpha = (int)(color.A * 255);

	//call TwSetParam
	TwSetParam(bar, NULL, "color", TW_PARAM_INT32, 3, intColor);
	TwSetParam(bar, NULL, "alpha", TW_PARAM_INT32, 1, &intAlpha);



	//---------------------------------------------------------�w���v���
	//TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar in vvvv.' "); // Message added to the help bar.
	//TwDefine("TW_HELP visible=false"); //help bar is hidden


	//---------------------------------------------------------�ϐ��̒ǉ�
	/*
	//bool test
	TwAddVarRW(bar, "toggle", TW_TYPE_BOOLCPP, &(this->unmanagedClass->toggleArray[0]), "label='Toggle'");
	//pin_ptr<bool> pBool = &FArrayToggle[0];
	//TwAddVarRW(bar, "toggle", TW_TYPE_BOOLCPP, pBool, " label='Toggle' ");
	//pBool = nullptr;

	//int test
	TwAddVarRW(bar, "int", TW_TYPE_INT32, &(this->unmanagedClass->integerArray[0]), "label='Int' min=0 max=1000");
	//pin_ptr<int> pInt = &FArrayInteger[0];
    //TwAddVarRW(bar, "Int", TW_TYPE_INT32, pInt, " label='Integer' min=1 max=1000 keyIncr=s keyDecr=S help='int variable test' ");
	//pInt = nullptr;
	
	//double test
	TwAddVarRW(bar, "double", TW_TYPE_DOUBLE, &(this->unmanagedClass->doubleArray[0]), "label='Double' step=0.1");
	//pin_ptr<double> pDouble = &FArrayDouble[0];
	//TwAddVarRW(bar, "double", TW_TYPE_DOUBLE, pDouble, " label='Double' step=0.1 ");
	//pDouble = nullptr;
	
	//���܂������Ȃ��H�H
	//pin_ptr<char*> pString = &FArrayString[0];
	//TwAddVarRW(bar, "String", TW_TYPE_CDSTRING, pString, "label='String'");
	//pString = nullptr;

	//color test
	TwAddVarRW(bar, "color", TW_TYPE_COLOR4F, &(this->unmanagedClass->colorArray[0][0]), "label='Color'");
	//pin_ptr<float> pColor = &FArrayColor[0, 0];
    //TwAddVarRW(bar, "color", TW_TYPE_COLOR4F, pColor, " label='Color' ");
	//pColor = nullptr;

	//direction test
	TwAddVarRW(bar, "dir", TW_TYPE_DIR3D, &(this->unmanagedClass->directionArray[0][0]), "label='Direction'");
	//pin_ptr<double> pDir = &FArrayDirection[0, 0];
	//TwAddVarRW(bar, "dir", TW_TYPE_DIR3D, pDir, " label='Direction' ");
	//pDir = nullptr;

	//quaternion test
	TwAddVarRW(bar, "quat", TW_TYPE_QUAT4D, &(this->unmanagedClass->quaternionArray[0][0]), "label='Quaternion'");
	//pin_ptr<double> pQuat = &FArrayQuaternion[0, 0];
	//TwAddVarRW(bar, "quat", TW_TYPE_QUAT4D, pQuat, "label='Quaternion'");
	//pQuat = nullptr;
	*/
}

//TweakBar�̃L�[�{�[�h�C�x���g���쐬
void TweakBarNode::CreateKeyEvent(int keyCodeWin32, int modifiers)
{
	int key = -1;	//�_�~�[�l���Z�b�g

	//Win32 API��MapVirtualKey���g���ƕ֗����ۂ�
	//http://msdn.microsoft.com/ja-jp/library/cc410909.aspx
	//user32.lib��Windows.h���K�v�H
	//UINT ui = MapVirtualKey(keyCodeWin32, 2);

	//Win32API�̉��z�L�[�R�[�h�ɉ����ď���
	//'('�Ƃ�'%'�Ƃ���Shift�L�[��������Ă��邩�ǂ����Ŕ��f���邵���Ȃ��H
	switch(keyCodeWin32)
	{
		case 8:		//BackSpace
			key = TW_KEY_BACKSPACE;
			break;
		case 9:		//Tab
			key = TW_KEY_TAB;
			break;
		case 13:	//Return
			key = TW_KEY_RETURN;
			break;
		case 19:	//Pause
			key = TW_KEY_PAUSE;
			break;
		case 27:	//Escape
			key = TW_KEY_ESCAPE;
			break;
		case 32:	//Space
			key = TW_KEY_SPACE;
			break;
		case 33:	//PageUp
			key = TW_KEY_PAGE_UP;
			break;
		case 34:	//PageDown
			key = TW_KEY_PAGE_DOWN;
			break;
		case 35:	//End
			key = TW_KEY_END;
			break;
		case 36:	//Home
			key = TW_KEY_HOME;
			break;
		case 37:	//LEFT
			key = TW_KEY_LEFT;
			break;
		case 38:	//UP
			key = TW_KEY_UP;
			break;
		case 39:	//RIGHT
			key = TW_KEY_RIGHT;
			break;
		case 40:	//DOWN
			key = TW_KEY_DOWN;
			break;
		case 45:	//Insert
			key = TW_KEY_INSERT;
			break;
		case 46:	//Delete
			key = TW_KEY_DELETE;
			break;
		case 48:	//0 - 9
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
		case 56:
		case 57:
			//ASCII�Ɠ���
			key = keyCodeWin32;
			break;
		case 65:	//A - Z
		case 66:
		case 67:
		case 68:
		case 69:
		case 70:
		case 71:
		case 72:
		case 73:
		case 74:
		case 75:
		case 76:
		case 77:
		case 78:
		case 79:
		case 80:
		case 81:
		case 82:
		case 83:
		case 84:
		case 85:
		case 86:
		case 87:
		case 88:
		case 89:
		case 90:
			//�������Ȃ��Ƒ啶���ɂȂ�Ȃ��B�Ȃ��H
			if(modifiers == TW_KMOD_SHIFT)
			{
				//���̂܂܁iASCII��65 - 90�j
				key = keyCodeWin32;
			}
			else
			{
				//�������ɕϊ��iASCII��97 - 122�j
				key = keyCodeWin32 + 32;
			}
			break;
		case 96:	//NUMPAD 0 - 9
		case 97:
		case 98:
		case 99:
		case 100:
		case 101:
		case 102:
		case 103:
		case 104:
		case 105:
			//0 - 9�ɕϊ��iASCII��48 - 57�j
			key = keyCodeWin32 - 48;
			break;
		case 106:	//NUMPAD *,+,,,-,.,/
		case 107:
		case 108:
		case 109:
		case 110:
		case 111:
			//ASCII��42 - 47�ɕϊ�
			key = keyCodeWin32 - 64;
			break;
		//F1 - F15�iWin32��F16 - F24�͖�������j
		case 112:
			key = TW_KEY_F1;
			break;
		case 113:
			key = TW_KEY_F2;
			break;
		case 114:
			key = TW_KEY_F3;
			break;
		case 115:
			key = TW_KEY_F4;
			break;
		case 116:
			key = TW_KEY_F5;
			break;
		case 117:
			key = TW_KEY_F6;
			break;
		case 118:
			key = TW_KEY_F7;
			break;
		case 119:
			key = TW_KEY_F8;
			break;
		case 120:
			key = TW_KEY_F9;
			break;
		case 121:
			key = TW_KEY_F10;
			break;
		case 122:
			key = TW_KEY_F11;
			break;
		case 123:
			key = TW_KEY_F12;
			break;
		case 124:
			key = TW_KEY_F13;
			break;
		case 125:
			key = TW_KEY_F14;
			break;
		case 126:
			key = TW_KEY_F15;
			break;
		case 186:	//:
			key = 58;
			break;
		case 187:	//;
			key = 59;
			break;
		case 188:	//,
			key = 44;
			break;
		case 189:	//-
			key = 45;
			break;
		case 190:	//.
			key = 46;
			break;
		case 191:	///
			key = 47;
			break;
		case 192:	//@
			key = 64;
			break;
		case 219:	//[
			key = 91;
			break;
		case 220:	//yen
			key = 92;
			break;
		case 221:	//]
			key = 93;
			break;
		case 222:	//^
			key = 94;
			break;
		case 223:	//_
			key = 95;
			break;
		case 226:	//_
			key = 95;
			break;
		default:
			//��L�ȊO�͈Ӗ����Ȃ��̂ŏI���
			break;
	}
	
	if(key > 0)
	{
		//call TweakBar event
		TwKeyPressed(key, modifiers);
	}
}

//�ϐ��̒ǉ�
void TweakBarNode::AddVariable(String^ type, String^ name, String^ param)
{
	if(FVariables->FindVariable(name))
	{
		//���łɓo�^�ς݂̖��O�̏ꍇ�͏I��
		FMessage[0] = name + " is already exists";

		return;
	}

	if(name == "" && type != "Separator")
	{
		//Separator�ȊO�Ŗ��O���Ȃ��ꍇ�͏I��
		FMessage[0] = "please input name";

		return;
	}



	//�ϐ���ǉ�����ꏊ��������
	int index = FVariables->GetEmptyIndex(type);

	if(index == -1)
	{
		//�o�^���������l�ɒB���Ă���̂Œǉ����Ȃ�
		FMessage[0] = type + " variables is full";

		return;
	}
	else
	{
		int result;
		char *label, *charName, *charParam;
		TwType twType;
		void *var;

		// System::String^����char*�֕ϊ�
		// 1)label
		label = (char *)Marshal::StringToHGlobalAnsi(name).ToPointer();
			
		// 2)internal name(type + index)
		charName = (char *)Marshal::StringToHGlobalAnsi(type + index.ToString()).ToPointer();

		// 3)optional parameters
		charParam = (char *)Marshal::StringToHGlobalAnsi(param).ToPointer();

			
		//�^��Separator�̏ꍇ�̂ݕʏ���
		if(type == "Separator")
		{
			//���O�̂Ȃ�Separator����
			if(name == "") charName = NULL;

			//TweakBar��Separator��ǉ�����
			result = TwAddSeparator(bar, charName, charParam);
		}
		else
		{
			//type�ɉ�����TweakBar�ɒǉ�����ϐ��̌^��ݒ肷��
			//switch�̏�����String^�͎g���Ȃ��̂�if�ɂ���
			if(type == "Toggle") twType = TW_TYPE_BOOLCPP;
			else if(type == "Integer") twType = TW_TYPE_INT32;
			else if(type == "Double") twType = TW_TYPE_DOUBLE;
			else if(type == "String") twType = TW_TYPE_CSSTRING(STR_LENGTH);	//STR_LENGTH defined TweakBarVariables.h
			else if(type == "Color") twType = TW_TYPE_COLOR4F;
			else if(type == "Direction") twType = TW_TYPE_DIR3D;
			else if(type == "Quaternion") twType = TW_TYPE_QUAT4D;
				
			//�ϐ��̃|�C���^���擾����i�ϐ��̏�������GetPointer()�ōs���j
			var = FVariables->GetPointer(type, index);

			//TweakBar�ɕϐ���ǉ�����
			result = TwAddVarRW(bar, charName, twType, var, charParam);
		}
			
		if(!result)
		{
			//���s
			FMessage[0] = gcnew String(TwGetLastError());
				
			//�I�v�V�����p�����[�^�������̏ꍇ�A�ϐ��͒ǉ�����Ă��܂��̂ł�����폜����
			//����ȊO�̏ꍇ�͎��s���邪�C�ɂ��Ȃ�
			TwRemoveVar(bar, charName);
		}
		else
		{
			//����
			FMessage[0] = "Add " + name + " successed";

			//label���Z�b�g
			if(type != "Separator") TwSetParam(bar, charName, "label", TW_PARAM_CSTRING, 1, label);

			//���O��index��o�^����i���O�̖���Separator�������j
			if(name != "") FVariables->AddVariable(type, name, index);
				

			//�o�̓s���̃Z�b�g
			/*
			FIntegerName.SliceCount = FVariables->FDictInteger->Count;
			for(int i = 0; i < FIntegerName.SliceCount; i++)
			{
				//Dictionary�̈ꗗ���o�͂���F�ǉ��������ɂł��邩�H
			}
			*/
		}

		//�������̉��
		Marshal::FreeHGlobal(IntPtr(label));
		Marshal::FreeHGlobal(IntPtr(charName));
		Marshal::FreeHGlobal(IntPtr(charParam));
	}
}


//�ϐ��̍폜
void TweakBarNode::RemoveVariable(String^ name)
{
	if(FVariables->FindVariable(name) == false)
	{
		//������Ȃ������ꍇ�͏I��
		FMessage[0] = name + " not found";
	}
	else
	{
		int index;

		//�ϐ����ɑΉ�����index���擾
		index = FVariables->GetIndex(name);

		if(index == -1)
		{
			//���s
			FMessage[0] = "get " + name + " failed";
		}
		else
		{
			char *charName;

			//�����p�̖��O���쐬����i�^�{ID�j
			charName = (char *)Marshal::StringToHGlobalAnsi(FVariables->GetType(name) + index.ToString()).ToPointer();
				
			//TweakBar����ϐ����폜����
			if(!TwRemoveVar(bar, charName))
			{
				//���s
				FMessage[0] = gcnew String(TwGetLastError());
			}
			else
			{
				//����
				FMessage[0] = "Remove " + name + " successed";

				//���O��index���폜����
				FVariables->RemoveVariable(name);
			}

			//�������̉��
			Marshal::FreeHGlobal(IntPtr(charName));
		}
	}
}


/*
//�}�l�[�W�ƃA���}�l�[�W��callBack�Ȃǂ�╡�G�Ȃ̂łƂ肠�����ۗ�
//TweakBar�̃{�^�����������Ƃ��ɌĂ΂�郁�\�b�h�i�e�X�g�p�j
void TW_CALL TweakBarDevelop::ButtonCallback(void *clientData)
{
	//do something here
}
*/