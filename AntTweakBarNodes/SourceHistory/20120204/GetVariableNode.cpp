#include "stdafx.h"

#include "GetVariableNode.h"

using namespace VVVV::Nodes;

GetVariableNode::GetVariableNode(IPluginHost2^ host)
{
	//IPluginHost2���t�B�[���h�ɃZ�b�g
	//�����FHost->GetID();�Ȃǂ��g����
	//FHost = host;

	//TweakBarDataType�̏�����
	//FTweakBarDataType = gcnew TweakBarDataType();

	//Output pin�̏�����
	/*
	FToggleOutput->SliceCount = 0;
	FIntegerOutput->SliceCount = 0;
	FDoubleOutput->SliceCount = 0;
	FStringOutput->SliceCount = 0;
	FColorOutput->SliceCount = 0;
	FDirectionOutput->SliceCount = 0;
	FQuaternionOutput->SliceCount = 0;
	*/
}

//called when data for any output pin is requested
void GetVariableNode::Evaluate(int SpreadMax)
{
	//FLogger->Log(LogType::Debug, "I am from Evaluate");
	
	if(FTweakBarInput->IsChanged)
    {
		if(FTweakBarInput[0] != nullptr)
		{
			FLogger->Log(LogType::Debug, "TweakBar connected");

			FTweakBarDataType = (TweakBarDataType^)FTweakBarInput[0];

			//TweakBar���擾���A�p�����[�^���擾����
			//TwBar *bar = ((TweakBarDataType^)FTweakBarInput[0])->GetTwBar();
			//String^ name = gcnew String(TwGetBarName(bar));
			//FLogger->Log(LogType::Debug, "name: " + name);
			
		}
		else
		{
			FTweakBarDataType = nullptr;

			FLogger->Log(LogType::Debug, "TweakBar disconnected");
		}
	}

	if(FToggleName->IsChanged)
	{
		if(FToggleName[0] != nullptr)
		{
			//TwGetParam(FTweakBarDataType->GetTwBar(), "toggle", 
			//FToggleOutput[0] = ;
		}
	}

	/*
	FOutput.SliceCount = SpreadMax;

	for (int i = 0; i < SpreadMax; i++)
	{
		FOutput[i] = FInput[i] * 2;
	}
	*/
}
