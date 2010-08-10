require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#eql?" do
  it "should be able determine equality between date objects" do
    DateTime.civil(2007, 10, 11).should eql(DateTime.civil(2007, 10, 11))
    DateTime.civil(2007, 10, 11, 10, 11, 12, 0.5).should eql(DateTime.civil(2007, 10, 11, 10, 11, 12, 0.5))
    DateTime.civil(2007, 10, 11, 10, 11, 12, 0.5).should eql(DateTime.civil(2007, 10, 12, 10, 11, 12, 0.5) - 1)
    DateTime.civil(2007, 10, 11, 10, 11, 12, 0.5).should_not eql(DateTime.civil(2007, 10, 11, 10, 11, 12, 0.4))
  end
end
