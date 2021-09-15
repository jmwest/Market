void make_matches(vector <Sellpq>* s_market, vector <Buypq>* b_market, Order* order,
				  vector <Client>* clients, Verbose &verbose, Median &median,
				  int &orders_processed, ostringstream &ss) {
	
	int current_client = order->get_client()->get_client_id();
	int current_equity = order->get_equity()->get_equity_id();
	
	if (order->get_transaction() == Order::BUY) {
		
		Sellpq* market_ptr = &s_market->at(current_equity);
		
		// remove any empty equity orders from the top of the pq.
		//		while (!market_ptr->empty()
		//			   && !market_ptr->top()->get_quantity()) {
		//			delete market_ptr->top();
		//			market_ptr->pop();
		//		}
		//
		//		Sellpq market_cpy = s_market->at(current_equity);
		queue <Order> market_cpy;
		
		while (order->get_quantity() && !market_ptr->empty()) {
			
			if (order->get_price() < market_ptr->top().get_price()) {
				break;
			}
			
			if (can_trade(order, &const_cast<Order&>(market_ptr->top()))) {
				++orders_processed;
				
				int market_client = market_ptr->top().get_client()->get_client_id();
				
				int equity_bought = (order->get_quantity() <= market_ptr->top().get_quantity()) ? order->get_quantity() : market_ptr->top().get_quantity();
				int price_bought = market_ptr->top().get_price();
				int total_spent = equity_bought * price_bought;
				
				order->change_quantity(equity_bought);
				const_cast<Order&>(market_ptr->top()).change_quantity(equity_bought);
				
				clients->at(current_client).add_bought(equity_bought);
				clients->at(current_client).add_net_value(-total_spent);
				clients->at(market_client).add_sold(equity_bought);
				clients->at(market_client).add_net_value(total_spent);
				
				if (median == YES_MEDIAN) {
					order->get_equity()->add_price(price_bought);
				}
				
				if (verbose == YES_VERBOSE) {
					output_verbose(current_client, market_client, current_equity, equity_bought,
								   price_bought, ss);
				} // if verbose == YES_VERBOSE
			} // if can_trade(order, market_cpy.top()
			
			if (const_cast<Order&>(market_ptr->top()).get_quantity()) {
				market_cpy.push(const_cast<Order&>(market_ptr->top()));
			}
			//			else {
			//				delete market_ptr->top();
			//			}
			
			market_ptr->pop();
		} // while
		
		while (!market_cpy.empty()) {
			//			if (market_cpy.front()->get_quantity()) {
			market_ptr->push(market_cpy.front());
			market_cpy.pop();
			//			}
			//			else {
			//				delete market_cpy.front();
			//				market_cpy.pop();
			//			}
		}
		
		if (order->get_quantity()) {
			b_market->at(current_equity).push(*order);
		} // if
		  //		else {
		  //			delete order; order = nullptr;
		  //		}
	}
	else {
		
		Buypq* market_ptr = &b_market->at(current_equity);
		// remove any empty equity orders from the top of the pq.
		//		while (!market_ptr->empty()
		//			   && !market_ptr->top()->get_quantity()) {
		//			delete market_ptr->top();
		//			market_ptr->pop();
		//		}
		//
		//		Buypq market_cpy = b_market->at(order->get_equity()->get_equity_id());
		queue <Order> market_cpy;
		
		while (!market_ptr->empty() && order->get_quantity()) {
			
			if (order->get_price() > market_ptr->top().get_price()) {
				break;
			}
			
			if (can_trade(order, &const_cast<Order&>(market_ptr->top()))) {
				++orders_processed;
				
				int market_client = market_ptr->top().get_client()->get_client_id();
				
				int equity_bought = (order->get_quantity() <= market_ptr->top().get_quantity()) ? order->get_quantity() : market_ptr->top().get_quantity();
				int price_bought = market_ptr->top().get_price();
				int total_spent = equity_bought * price_bought;
				
				order->change_quantity(equity_bought);
				const_cast<Order&>(market_ptr->top()).change_quantity(equity_bought);
				
				clients->at(current_client).add_sold(equity_bought);
				clients->at(current_client).add_net_value(total_spent);
				clients->at(market_client).add_bought(equity_bought);
				clients->at(market_client).add_net_value(-total_spent);
				
				if (median == YES_MEDIAN) {
					order->get_equity()->add_price(price_bought);
				}
				
				if (verbose == YES_VERBOSE) {
					output_verbose(market_client, current_client, current_equity, equity_bought, price_bought, ss);
				} // if verbose == YES_VERBOSE
			} // if can_trade(order, market_cpy.top()
			
			if (market_ptr->top().get_quantity()) {
				market_cpy.push(market_ptr->top());
			}
			//			else {
			//				delete market_ptr->top();
			//			}
			
			market_ptr->pop();
		} // while
		
		while (!market_cpy.empty()) {
			//			if (market_cpy.front()->get_quantity()) {
			market_ptr->push(market_cpy.front());
			market_cpy.pop();
			//			}
			//			else {
			//				delete market_cpy.front();
			//				market_cpy.pop();
			//			}
		}
		
		if (order->get_quantity()) {
			s_market->at(current_equity).push(*order);
		} // if
		  //		else {
		  //			delete order; order = nullptr;
		  //		}
	}
	
	return;
} // make_matches
