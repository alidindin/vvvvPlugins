// 
// GetVariableNode.h
// 
// get variables of AntTweakBar
// 
// updated: 2012/02/05
// written: Minoru Ito

#pragma once

//----------------------�K�v�Ȗ��O��Ԃ�錾
//�ǂ��g�����̂�Stdafx.h�Ɉړ�



//TweakBar�̕ϐ���Pin�ň�����悤�ɂ���
#include "TweakBarVariables.h"
using namespace VVVV::DataTypes;



namespace VVVV
{
	namespace Nodes
	{
		//PluginInfo�̐ݒ�i���O�A�J�e�S���A�w���v�A�^�O�Ȃǁj
		[PluginInfo(Name = "GetVariable", Category = "AntTweakBar", Help = "", Tags = "", Author = "mino")]
		//�N���X�錾
		public ref class GetVariableNode : IPluginEvaluate
		{

		public:

			//�R���X�g���N�^
			[ImportingConstructor()]
			GetVariableNode(IPluginHost2^ host);

			
			//----------------------�K�{��public���\�b�h
			virtual void Evaluate(int SpreadMax);

		protected:

			//

		private:

			//---------------------------------���̓s��
			
			//TweakBar�̕ϐ����������߂̃N���X
			TweakBarVariables^ FVariables;
			
			[Input("TweakBar Variables", IsSingle = true)]
			IDiffSpread<TweakBarVariables^>^ FVariablesInput;

			[Input("Toggle Name")]
			IDiffSpread<String^>^ FToggleName;

			[Input("Integer Name")]
			IDiffSpread<String^>^ FIntegerName;

			[Input("Double Name")]
			IDiffSpread<String^>^ FDoubleName;

			[Input("String Name")]
			IDiffSpread<String^>^ FStringName;

			[Input("Color Name")]
			IDiffSpread<String^>^ FColorName;

			[Input("Direction Name")]
			IDiffSpread<String^>^ FDirectionName;

			[Input("Quaternion Name")]
			IDiffSpread<String^>^ FQuaternionName;


			//---------------------------------�o�̓s��

			[Output("Test String")]
			ISpread<String^>^ FTestString;

			[Output("Toggle")]
			ISpread<bool>^ FToggleOutput;

			[Output("Integer")]
			ISpread<int>^ FIntegerOutput;

			[Output("Double")]
			ISpread<double>^ FDoubleOutput;

			[Output("String")]
			ISpread<String^>^ FStringOutput;

			[Output("Color")]
			ISpread<RGBAColor>^ FColorOutput;

			[Output("Direction")]
			ISpread<Vector3D>^ FDirectionOutput;

			[Output("Quaternion")]
			ISpread<Vector4D>^ FQuaternionOutput;


			//----------------------ILogger
			[Import()]
			ILogger^ FLogger;


			//----------------------���̑��̃t�B�[���h
			
			//slice count
			//int FSpreadCount;
		};
	}
}