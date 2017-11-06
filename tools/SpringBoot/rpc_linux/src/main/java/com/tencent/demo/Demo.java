package com.tencent.demo;

import com.tencent.engine.TmeAction;
import com.tencent.engine.Function;
import com.tencent.engine.PreProcHandler;
import com.tencent.engine.RegistTransMachine;
import com.tencent.engine.Service;
import com.tencent.engine.TccAction;
import com.tencent.engine.TransMachine;
import com.tencent.engine.Transition;
import com.tencent.operators.OperatorPath;
import com.tencent.operators.OperatorType;
import com.tencent.operators.OperatorValue;
import com.tencent.operators.TmeOperator;
import com.tencent.rpc.App;



public class Demo {
	
	
	public static TransMachine RegistTestRoutine() throws Exception
	{
		App.logInstance().info("RegistTestRoutine");
		
		TransMachine mch = new TransMachine("routine1");
		
		// node a
		Service tcc_a = new Service("tcc_a");
		Function a_try = tcc_a.addFunction("tcc_a_try", 
				new PreProcHandler() {
			        public int preProcIn(Object str) {
			        	return 0;
			        }
                    public int preProcOut(Object str) {
			        	return 0;
			        }
			    });
		Function a_confirm = tcc_a.addFunction("tcc_a_confirm");
		Function a_cancel = tcc_a.addFunction("tcc_a_cancel");
		
		TccAction act_a = new TccAction("tcc_a"/*Service Name*/, 
				a_try/*Function*/, a_confirm/*Function*/, a_cancel/*Function*/);
		
		// node b
		Service tcc_b = new Service("tcc_b");
		TccAction act_b = new TccAction("tcc_b", 
				tcc_b.addFunction("tcc_b_try"), tcc_b.addFunction("tcc_b_confirm"), tcc_b.addFunction("tcc_b_cancel"));
		
		mch.addAction(act_a);
		mch.addAction(act_b);
		
		mch.addTransition(new Transition(TmeAction.BEGIN, act_a, new TmeOperator(OperatorType.ANY)));
		mch.addTransition(new Transition(act_a, act_b, new TmeOperator(OperatorType.EQ, new OperatorPath("OUT", "ret_code"), new OperatorValue("0"))));
		mch.addTransition(new Transition(act_a, TmeAction.ROLLBACK, new TmeOperator(OperatorType.NE, new OperatorValue("0"))));
		mch.addTransition(new Transition(act_b, TmeAction.COMMIT, new TmeOperator(OperatorType.EQ, new OperatorValue("0"))));
		mch.addTransition(new Transition(act_b, TmeAction.ROLLBACK, new TmeOperator(OperatorType.NE, new OperatorValue("0"))));
		mch.addTransition(new Transition(TmeAction.COMMIT, TmeAction.END, new TmeOperator(OperatorType.ANY)));
		mch.addTransition(new Transition(TmeAction.ROLLBACK, TmeAction.END, new TmeOperator(OperatorType.ANY)));
		
		return mch;
	}
}
