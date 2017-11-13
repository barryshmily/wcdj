package com.tencent.operators;

import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class TmeOperator extends Operator {
	private static final Logger log = LoggerFactory.getLogger("RunLogger");

	public TmeOperator(OperatorType type_, OperatorPath path_,
			OperatorValue value_) {
		if (type_ == null) {
			// throw ...
		}
		if (path_ == null
				|| (path_.getPath().size() > 3 || path_.getPath().size() < 2)) {
			// throw ...
		}
		if (value_ == null) {
			// throw ...
		}
		type = type_;
		path = path_;
		value = value_;
	}

	public TmeOperator(OperatorType type_, OperatorValue value_) {
		if (type_ == null) {
			// throw ...
		}
		if (value_ == null) {
			// throw ...
		}
		type = type_;
		value = value_;

		path = new OperatorPath("");
	}

	public TmeOperator(OperatorType type_) {
		if (type_ != OperatorType.ANY) {
			// throw ...
		}
		type = type_;

		value = new OperatorValue("-");
		path = new OperatorPath("");
	}

	// @Override
	public OperatorType getType() {
		return type;
	}

	// @Override
	public List<String> getPath() {
		return path.getPath();

	}

	// @Override
	public List<Object> getValue() {
		//log.info("TmeOperator getValue");
		return value.getValue();
	}
}
